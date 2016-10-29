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
#include "../../core/repository/consensus/event_repository.hpp"
#include "../../core/model/transactions/transfer_transaction.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>

#include "../../core/service/peer_service.hpp"

int main(){
    std::string cmd;
    std::vector<std::unique_ptr<peer::Node>> nodes = peer::getPeerList();

    for(const auto& n : nodes){
        std::cout<< "=========" << std::endl;
        std::cout<< n->getPublicKey() << std::endl;
        std::cout<< n->getIP() << std::endl;
    }

    std::string pubKey = "UKsNazVWDZ1oqNBoFpckirYDvwI/LyL9BqNYXVWCqmw=";
    sumeragi::initializeSumeragi( pubKey, std::move(nodes));
    std::thread http_th( []() {
        sumeragi::loop();
    });

    while(1){
        std::cout << "in >> ";
        std::cin>> cmd;
        if(cmd == "quit") break;
        repository::event::add("80084bf2fba02475726feb2cab2d8215eab14bc6bdd8bfb2c8151257032ecd8b",
            std::make_unique<consensus_event::ConsensusEvent>(
                std::make_unique<transaction::TransferTransaction>(
                    "fccpkrZyLlxJUQm8RpJXedWVZfbg2Dde0iPphwD+jQ0=",
                    pubKey,
                    "domain",
                    cmd
                )
            )
        );

    }

    http_th.detach();

    return 0;
}
