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
// Created by Takumi Yamashita on 2017/03/09.
//

#include <iostream>
#include <string>
#include <infra/config/peer_service_with_json.hpp>
#include <service/peer_service.hpp>


int main(int argc, char* argv[]){
    std::string ip = config::PeerServiceConfig::getInstance().getMyIp();
    std::string pubkey = config::PeerServiceConfig::getInstance().getMyPublicKey();
    if( argc == 3 ) {
        ip = argv[1];
        pubkey = argv[2];
    }

    peer::Node peer( ip, pubkey );
    config::PeerServiceConfig::getInstance().toIssue_addPeer( peer );
}