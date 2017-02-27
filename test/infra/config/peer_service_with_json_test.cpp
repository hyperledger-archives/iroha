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

//
// Created by Takumi Yamashita on 2017/02/23.
//


#include <gtest/gtest.h>
#include <vector>
#include "../../../core/infra/config/peer_service_with_json.hpp"
#include "../../../core/service/peer_service.hpp"

TEST( peer_service_with_json_test, initialize_peer_test ) {
    std::vector<std::unique_ptr<peer::Node>> peers = config::PeerServiceConfig::getInstance().getPeerList();
    for( auto&& peer : peers ) {
        std::cout << peer->getIP() << std::endl;
        std::cout << peer->getPublicKey() << std::endl;
        std::cout << peer->getTrustScore() << std::endl;
    }
    ASSERT_TRUE( peers.size() == 4 );
}

TEST( peer_service_with_json_test, add_peer_test ) {
    int n = config::PeerServiceConfig::getInstance().getPeerList().size();
    peer::Node peer1 = peer::Node( "ip_low", "publicKey", 0.5 );
    peer::Node peer2 = peer::Node( "ip_high", "publicKey", 1.5 );
    ASSERT_TRUE( config::PeerServiceConfig::getInstance().addPeer( peer1 ) );
    ASSERT_FALSE( config::PeerServiceConfig::getInstance().addPeer( peer2 ) );
    std::vector<std::unique_ptr<peer::Node>> peers = config::PeerServiceConfig::getInstance().getPeerList();
    for( auto&& peer : peers ) {
        std::cout << peer->getIP() << std::endl;
        std::cout << peer->getPublicKey() << std::endl;
        std::cout << peer->getTrustScore() << std::endl;
    }
    ASSERT_TRUE( peers.size() == n+1 );
}

TEST( peer_service_with_json_test, update_peer_test ) {
    int n = config::PeerServiceConfig::getInstance().getPeerList().size();
    const std::string upd_ip = "172.17.0.3";
    const std::string upd_key = "jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=";
    const std::string upd_ng_key = "dummy";
    peer::Node peer = peer::Node( upd_ip, upd_key, 0.1 );
    peer::Node peer_ng = peer::Node( upd_ip, upd_ng_key, 0.1 );
    ASSERT_TRUE( config::PeerServiceConfig::getInstance().updatePeer( peer ) );
    ASSERT_FALSE( config::PeerServiceConfig::getInstance().updatePeer( peer_ng ) );
    std::vector<std::unique_ptr<peer::Node>> peers = config::PeerServiceConfig::getInstance().getPeerList();
    for( auto&& peer : peers ) {
        std::cout << peer->getIP() << std::endl;
        std::cout << peer->getPublicKey() << std::endl;
        std::cout << peer->getTrustScore() << std::endl;
        if( peer->getIP() == upd_ip && peer->getPublicKey() == upd_key )
            ASSERT_TRUE( peer->getTrustScore() == 0.1 );
    }
    ASSERT_TRUE( peers.size() == n );
}

TEST( peer_service_with_json_test, remove_peer_test ) {
    int n = config::PeerServiceConfig::getInstance().getPeerList().size();
    peer::Node peer = peer::Node( "172.17.0.3", "jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=" );
    ASSERT_TRUE( config::PeerServiceConfig::getInstance().removePeer( peer ) );
    ASSERT_FALSE( config::PeerServiceConfig::getInstance().removePeer( peer ) );
    std::vector<std::unique_ptr<peer::Node>> peers = config::PeerServiceConfig::getInstance().getPeerList();
    for( auto&& peer : peers ) {
        std::cout << peer->getIP() << std::endl;
        std::cout << peer->getPublicKey() << std::endl;
        std::cout << peer->getTrustScore() << std::endl;
    }
    ASSERT_TRUE( peers.size() == n-1 );
}


TEST( peer_service_with_json_test, leder_peer_check_test ) {
    std::vector<std::unique_ptr<peer::Node>> peers = config::PeerServiceConfig::getInstance().getPeerList();
    ASSERT_FALSE( config::PeerServiceConfig::getInstance().isLeaderMyPeer() );
    for( auto &&peer : peers ) {
        if( peer->getIP() != "172.17.0.6" ) {
            ASSERT_TRUE( config::PeerServiceConfig::getInstance().removePeer( *peer ) );
        }
    }
    ASSERT_TRUE( config::PeerServiceConfig::getInstance().isLeaderMyPeer() );
}

