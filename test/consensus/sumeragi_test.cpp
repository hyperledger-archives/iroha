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

#include "../../core/consensus/sumeragi.hpp"

#include "../../core/consensus/connection/connection.hpp"
#include "../../core/model/commands/transfer.hpp"
#include "../../core/model/objects/domain.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>

#include "../../core/service/json_parse_with_json_nlohmann.hpp"

#include "../../core/service/peer_service.hpp"
#include "../../core/crypto/hash.hpp"

int main(){
    std::string cmd;
    std::vector<std::unique_ptr<peer::Node>> nodes = peer::getPeerList();


    connection::initialize_peer(nullptr);

    for(const auto& n : nodes){
        std::cout<< "=========" << std::endl;
        std::cout<< n->getPublicKey() << std::endl;
        std::cout<< n->getIP() << std::endl;
        connection::addPublication(n->getIP());
    }

    std::string pubKey = peer::getMyPublicKey();

    sumeragi::initializeSumeragi( pubKey, std::move(nodes));

    std::thread http_th( []() {
        sumeragi::loop();
    });

    connection::exec_subscription(peer::getMyIp());
    connection::receive([](std::string from, std::string message){
        std::cout <<" receive :" << message <<" from:"<< from << "\n";
    });

    while(1){
        std::cout << "name  in >> ";
        std::cin>> cmd;
        if(cmd == "quit") break;

        for(const auto& n : nodes){
            std::cout<< "=========" << std::endl;
            std::cout<< n->getPublicKey() << std::endl;
            std::cout<< n->getIP() << std::endl;
        }

        auto event = consensus_event::ConsensusEvent<
                transaction::Transaction<command::Transfer<domain::Domain>>,
                command::Transfer<domain::Domain>
        >(
             transaction::Transaction<command::Transfer<domain::Domain>>(
                command::Transfer<domain::Domain>(
                   domain::Domain( peer::getMyPublicKey(), "cmd")
                )
             )
        );
        auto parser = json_parse_with_json_nlohman::JsonParse<
           consensus_event::ConsensusEvent<
              transaction::Transaction<command::Transfer<domain::Domain>>,
              command::Transfer<domain::Domain>
           >,
           transaction::Transaction<command::Transfer<domain::Domain>>,
           command::Transfer<domain::Domain>
        >();
        connection::send( peer::getMyIp(),  parser.dump(event.dump()));

    }

    http_th.detach();

    return 0;
}
