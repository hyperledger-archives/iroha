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
#include "peer_service_with_json.hpp"
#include "config_format.hpp"
#include "../../util/exception.hpp"

using PeerServiceConfig = config::PeerServiceConfig;
using nlohmann::json;

std::vector<peer::Node> PeerServiceConfig::peerList;

PeerServiceConfig::PeerServiceConfig() {}

void PeerServiceConfig::initialziePeerList_from_json(){
  if (!peerList.empty()) return;
  if (auto config = openConfig(getConfigName())) {
    for (const auto& peer : (*config)["group"].get<std::vector<json>>()) {
      peerList.emplace_back( peer["ip"].get<std::string>(),
                             peer["publicKey"].get<std::string>(),
                             1.0);
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
  return "";
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
  return "";
}

bool PeerServiceConfig::isExistIP( const std::string &ip ) {
  return findPeerIP( std::move(ip) ) != peerList.end();
}
bool PeerServiceConfig::isExistPublicKey( const std::string &publicKey ) {
  return findPeerPublicKey( std::move(publicKey) ) != peerList.end();
}

std::vector<peer::Node>::iterator PeerServiceConfig::findPeer( peer::Node &peer ) {
  return findPeerPublicKey( peer.getPublicKey() );
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

bool PeerServiceConfig::addPeer( peer::Node &peer ) {
  try {
    if( isExistIP( peer.getIP() ) )
      throw exception::service::DuplicationIPAddPeerException(std::move(peer.getIP()), std::move(peer.getPublicKey()), std::move(peer.getTrustScore()));
    if( isExistPublicKey( peer.getPublicKey() ) )
      throw exception::service::DuplicationPublicKeyAddPeerException(std::move(peer.getIP()), std::move(peer.getPublicKey()), std::move(peer.getTrustScore()));
    peerList.emplace_back( std::move(peer));
  } catch( exception::service::DuplicationPublicKeyAddPeerException e ) {
    logger::warning("addPeer") << e.what();
    return false;
  } catch( exception::service::DuplicationIPAddPeerException e ) {
    logger::warning("addPeer") << e.what();
    return false;
  }
  connection::iroha::Sumeragi::Verify::addSubscriber( peer.getIP() );
  return true;
}

bool PeerServiceConfig::removePeer( peer::Node &peer ) {
  try {
    auto it = findPeerPublicKey(peer.getPublicKey());
    if (it == peerList.end())
      throw exception::service::UnExistFindPeerException(std::move(peer.getIP()), std::move(peer.getPublicKey()), std::move(peer.getTrustScore()));
    peerList.erase(it);
  } catch (exception::service::UnExistFindPeerException e) {
    logger::warning("removePeer") << e.what();
    return false;
  }
  return true;
}

bool PeerServiceConfig::updatePeer( peer::Node &peer ) {
  try {
    auto it = findPeerPublicKey(peer.getPublicKey());
    if (it == peerList.end())
      throw exception::service::UnExistFindPeerException(std::move(peer.getIP()), std::move(peer.getPublicKey()), std::move(peer.getTrustScore()));
    peerList.erase(it);
    addPeer( peer );
  } catch ( exception::service::UnExistFindPeerException e ) {
    logger::warning("updatePeer") << e.what();
    return false;
  }
  connection::iroha::Sumeragi::Verify::addSubscriber( peer.getIP() );
  return true;
}

bool PeerServiceConfig::isLeaderMyPeer() {
  if( peerList.empty() ) return false;
  auto sorted_peers = getPeerList();
  return (*sorted_peers.begin())->getPublicKey() == getMyPublicKey() &&
         (*sorted_peers.begin())->getIP() == getMyIp();
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
