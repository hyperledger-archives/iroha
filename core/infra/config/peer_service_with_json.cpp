/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <deque>
#include <regex>
#include <algorithm>
#include <crypto/base64.hpp>
#include <util/logger.hpp>
#include <util/exception.hpp>
#include <util/use_optional.hpp>
#include <consensus/connection/connection.hpp>
#include <json.hpp>
#include <infra/protobuf/api.pb.h>
#include <transaction_builder/transaction_builder.hpp>
#include "peer_service_with_json.hpp"
#include "config_format.hpp"
#include "../../util/exception.hpp"

using PeerServiceConfig = config::PeerServiceConfig;
using txbuilder::TransactionBuilder;
using type_signatures::Update;
using type_signatures::Add;
using type_signatures::Remove;
using type_signatures::Peer;
using nlohmann::json;

std::vector<peer::Node> PeerServiceConfig::peerList;

PeerServiceConfig::PeerServiceConfig() {
    initialziePeerList_from_json();
}

void PeerServiceConfig::initialziePeerList_from_json(){
  if (!peerList.empty()) return;
  if (auto config = openConfig(getConfigName())) {
    for (const auto& peer : (*config)["group"].get<std::vector<json>>()) {
      peerList.emplace_back( peer["ip"].get<std::string>(),
                             peer["publicKey"].get<std::string>(),
                             getMaxTrustScore());
    }
  }
}

PeerServiceConfig& PeerServiceConfig::getInstance() {
  static PeerServiceConfig serviceConfig;
  return serviceConfig;
}

std::string PeerServiceConfig::getMyPublicKey() {
  if (auto config = openConfig(getConfigName())) {
    return (*config)["me"]["publicKey"].get<std::string>();
  }
  return peer::Node::defaultPublicKey();
}

std::string PeerServiceConfig::getMyPrivateKey() {
  if (auto config = openConfig(getConfigName())) {
    return (*config)["me"]["privateKey"].get<std::string>();
  }
  return "";
}

std::string PeerServiceConfig::getMyIp() {
  if (auto config = openConfig(getConfigName())) {
    return (*config)["me"]["ip"].get<std::string>();
  }
  return peer::Node::defaultIP();
}

double PeerServiceConfig::getMaxTrustScore() {
    return std::max( 10.0, (double)peerList.size() ); // temporary MaxTrustScore is max( 10, peerList.size() )
}

bool PeerServiceConfig::isExistIP( const std::string &ip ) {
  return findPeerIP( std::move(ip) ) != peerList.end();
}
bool PeerServiceConfig::isExistPublicKey( const std::string &publicKey ) {
  return findPeerPublicKey( std::move(publicKey) ) != peerList.end();
}

std::vector<peer::Node>::iterator PeerServiceConfig::findPeerIP( const std::string &ip ) {
  return std::find_if( peerList.begin(), peerList.end(),
                       [&ip]( const peer::Node& p ) { return p.getIP() == ip; } );
}

std::vector<peer::Node>::iterator PeerServiceConfig::findPeerPublicKey( const std::string &publicKey ) {
  return std::find_if( peerList.begin(), peerList.end(),
                       [&publicKey]( const peer::Node& p ) { return p.getPublicKey() == publicKey; } );
}


std::vector<std::unique_ptr<peer::Node>> PeerServiceConfig::getPeerList() {
  initialziePeerList_from_json();

  std::vector<std::unique_ptr<peer::Node>> nodes;
  for( auto &&node : peerList )
    nodes.push_back( std::make_unique<peer::Node>( node.getIP(), node.getPublicKey(), node.getTrustScore() ) );
  sort( nodes.begin(), nodes.end(),
        []( const std::unique_ptr<peer::Node> &a, const std::unique_ptr<peer::Node> &b ) { return a->getTrustScore() > b->getTrustScore(); } );
  return nodes;
}
std::vector<std::string> PeerServiceConfig::getIpList() {
  std::vector<std::string> ret_ips;
  for( auto &&node : peerList )
    ret_ips.push_back( node.getIP() );
  return ret_ips;
}


// check are broken? peer
void PeerServiceConfig::checkBrokenPeer( const std::string& ip ) {
    if (!isExistIP(ip)) return;
    auto check_peer_it = findPeerIP( ip );
    if ( !connection::iroha::PeerService::Sumeragi::ping( ip )) {
        if( check_peer_it->getTrustScore() < 0.0 ) {
            toIssue_removePeer( check_peer_it->getPublicKey() );
        } else {
            toIssue_distructPeer( check_peer_it->getPublicKey() );
        }
    } else {
        toIssue_creditPeer( check_peer_it->getPublicKey() );
    }
}

void PeerServiceConfig::finishedInitializePeer() {
    std::string leader_ip = leaderPeer()->getIP();
    auto txPeer = TransactionBuilder<Update<Peer>>()
            .setSenderPublicKey(getMyPublicKey())
            .setPeer(txbuilder::createPeer( getMyPublicKey(), getMyIp(), txbuilder::createTrust(0.0, true)))
            .build();
    connection::iroha::PeerService::Sumeragi::send( leader_ip, txPeer );
}

// invoke to issue transaction
void PeerServiceConfig::toIssue_addPeer( const peer::Node& peer ) {
    if( isExistIP(peer.getIP()) || isExistPublicKey(peer.getPublicKey()) ) return;
    auto txPeer = TransactionBuilder<Add<Peer>>()
            .setSenderPublicKey(getMyPublicKey())
            .setPeer( txbuilder::createPeer( peer.getPublicKey(), peer.getIP(), txbuilder::createTrust(getMaxTrustScore(),false ) ) )
            .build();
    connection::iroha::PeerService::Sumeragi::send( getMyPublicKey(), txPeer );
}
void PeerServiceConfig::toIssue_distructPeer( const std::string &publicKey ) {
    if( !isExistPublicKey(publicKey) ) return;
    auto txPeer = TransactionBuilder<Update<Peer>>()
            .setSenderPublicKey(getMyPublicKey())
            .setPeer(txbuilder::createPeer(publicKey, peer::Node::defaultIP(), txbuilder::createTrust(-1.0, true)))
            .build();
    connection::iroha::PeerService::Sumeragi::send( getMyPublicKey(), txPeer );
}
void PeerServiceConfig::toIssue_removePeer( const std::string &publicKey ) {
    if( !isExistPublicKey(publicKey) ) return;
    auto txPeer = TransactionBuilder<Remove<Peer>>()
            .setSenderPublicKey(getMyPublicKey())
            .setPeer(txbuilder::createPeer(publicKey, peer::Node::defaultIP(), txbuilder::createTrust(-getMaxTrustScore(), false)))
            .build();
    connection::iroha::PeerService::Sumeragi::send( getMyPublicKey(), txPeer );
}
void PeerServiceConfig::toIssue_creditPeer( const std::string &publicKey ) {
    if( !isExistPublicKey(publicKey) ) return;
    if( findPeerPublicKey(publicKey)->getTrustScore() == getMaxTrustScore() ) return;
    auto txPeer = TransactionBuilder<Update<Peer>>()
            .setSenderPublicKey(getMyPublicKey())
            .setPeer(txbuilder::createPeer(publicKey, peer::Node::defaultIP(),
                                           txbuilder::createTrust( +1.0, true)))
            .build();
    connection::iroha::PeerService::Sumeragi::send( getMyPublicKey(), txPeer );
}



bool PeerServiceConfig::addPeer( const peer::Node &peer ) {
  try {
    if( isExistIP( peer.getIP() ) )
      throw exception::service::DuplicationIPException(peer.getIP());
    if( isExistPublicKey( peer.getPublicKey() ) )
      throw exception::service::DuplicationPublicKeyException(peer.getPublicKey());
    peerList.emplace_back( std::move(peer));
  } catch( exception::service::DuplicationPublicKeyException& e ) {
    logger::warning("addPeer") << e.what();
    return false;
  } catch( exception::service::DuplicationIPException& e ) {
    logger::warning("addPeer") << e.what();
    return false;
  }


//  TODO Send transaction data separated block to new peer.
//  TODO connection::iroha::PeerService::TransactionRepository::send( peer.getIP() );
    while( false ) { // all - transaction
        auto tx = Api::TransactionResponse();
        while( false ) { // block
            tx.add_transaction()->CopyFrom(tx);
        }
        connection::iroha::PeerService::Sumeragi::send( peer.getIP(), tx );
    }
    return true;
}

bool PeerServiceConfig::removePeer( const std::string& publicKey ) {
  try {
    auto it = findPeerPublicKey( publicKey );
    if ( !isExistPublicKey( publicKey ) )
      throw exception::service::UnExistFindPeerException(publicKey);
    peerList.erase(it);
  } catch (exception::service::UnExistFindPeerException& e) {
    logger::warning("removePeer") << e.what();
    return false;
  }
  return true;
}

bool PeerServiceConfig::updatePeer( const std::string& publicKey, const peer::Node& peer ) {
  try {
    auto it = findPeerPublicKey( publicKey );
    if (it == peerList.end() )
      throw exception::service::UnExistFindPeerException( publicKey );

    if ( !peer.isDefaultPublicKey() ) {
      auto upd_it = findPeerPublicKey(peer.getPublicKey());
      if( upd_it != it && upd_it != peerList.end() ) throw exception::service::DuplicationPublicKeyException(peer.getPublicKey());
      it->setPublicKey( peer.getPublicKey() );
    }

    if ( !peer.isDefaultIP() ) {
        auto upd_it = findPeerIP(peer.getIP());
        if( upd_it != it && upd_it != peerList.end() ) throw exception::service::DuplicationIPException(peer.getIP());
        it->setIP( peer.getIP() );
    }

    if ( it->getTrustScore() != 0.0 ) {
        it->setTrustScore( std::min( getMaxTrustScore(), it->getTrustScore()+peer.getTrustScore()) );
    }

    if( it->isOK() != peer.isOK() ) {
        it->setOK( peer.isOK() );
    }

  } catch ( exception::service::UnExistFindPeerException& e ) {
    logger::warning("updatePeer") << e.what();
    return false;
  } catch( exception::service::DuplicationPublicKeyException& e ) {
      logger::warning("updatePeer") << e.what();
    return false;
  } catch ( exception::service::DuplicationIPException& e ) {
    logger::warning("updatePeer") << e.what();
    return false;
  }
  return true;
}


bool PeerServiceConfig::validate_addPeer( const peer::Node& peer ) {
    try {
        if( isExistIP( peer.getIP() ) )
            throw exception::service::DuplicationIPException(std::move(peer.getIP()));
        if( isExistPublicKey( peer.getPublicKey() ) )
            throw exception::service::DuplicationPublicKeyException(std::move(peer.getPublicKey()));
    } catch( exception::service::DuplicationPublicKeyException& e ) {
        logger::warning("validate addPeer") << e.what();
        return false;
    } catch( exception::service::DuplicationIPException& e ) {
        logger::warning("validate addPeer") << e.what();
        return false;
    }
    return true;
}
bool PeerServiceConfig::validate_removePeer( const std::string &publicKey ) {
    try {
        if ( !isExistPublicKey( publicKey ) )
            throw exception::service::UnExistFindPeerException(publicKey);
    } catch (exception::service::UnExistFindPeerException& e) {
        logger::warning("validate removePeer") << e.what();
        return false;
    }
    return true;
}
bool PeerServiceConfig::validate_updatePeer( const std::string& publicKey, const peer::Node& peer ) {
    try {
        auto it = findPeerPublicKey( publicKey );
        if (it == peerList.end() )
            throw exception::service::UnExistFindPeerException( publicKey );

        if ( !peer.isDefaultPublicKey() ) {
            auto upd_it = findPeerPublicKey(peer.getPublicKey());
            if( upd_it != it && upd_it != peerList.end() ) throw exception::service::DuplicationPublicKeyException(peer.getPublicKey());
        }

        if ( !peer.isDefaultIP() ) {
            auto upd_it = findPeerIP(peer.getIP());
            if( upd_it != it && upd_it != peerList.end() ) throw exception::service::DuplicationIPException(peer.getIP());
        }

    } catch ( exception::service::UnExistFindPeerException& e ) {
        logger::warning("updatePeer") << e.what();
        return false;
    } catch( exception::service::DuplicationPublicKeyException& e ) {
        logger::warning("updatePeer") << e.what();
    } catch( exception::service::DuplicationIPException& e ) {
        logger::warning("updatePeer") << e.what();
        return false;
    }
    return true;
}


bool PeerServiceConfig::isLeaderMyPeer() {
  auto sorted_peers = getPeerList();
  if( sorted_peers.empty() ) return false;
  return (*sorted_peers.begin())->getPublicKey() == getMyPublicKey() &&
         (*sorted_peers.begin())->getIP() == getMyIp();
}

std::unique_ptr<peer::Node> PeerServiceConfig::leaderPeer() {
  auto soreted_peers = getPeerList();
  if( soreted_peers.empty() ) return NULL;
  return std::move( *soreted_peers.begin() );
}

void PeerServiceConfig::parseConfigDataFromString(std::string&& jsonStr) {
  try {
    if (not ConfigFormat::getInstance().ensureFormatSumeragi(jsonStr)) {
      throw exception::ParseFromStringException("sumeragi");
    }
    _configData = json::parse(std::move(jsonStr));
  } catch (exception::ParseFromStringException& e) {
    logger::warning("peer service config") << e.what();
    logger::warning("peer service config") << getConfigName() << " is set to be default.";

    // default sumeragi.json
    _configData = json::parse(R"({
      "me":{
        "ip":"172.17.0.6",
        "name":"samari",
        "publicKey":"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=",
        "privateKey":"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA=="
      },
      "group":[
        {
          "ip":"172.17.0.3",
          "name":"mizuki",
          "publicKey":"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0="
        },
        {
          "ip":"172.17.0.4",
          "name":"natori",
          "publicKey":"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY="
        },
        {
          "ip":"172.17.0.5",
          "name":"kabohara",
          "publicKey":"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig="
        },
        {
          "ip":"172.17.0.6",
          "name":"samari",
          "publicKey":"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU="
        }
      ]
    })");
  }
}

std::string PeerServiceConfig::getConfigName() {
  return "config/sumeragi.json";
}
