/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp
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

#include <infra/protobuf/api.pb.h>
#include <iostream>
#include <infra/config/peer_service_with_json.hpp>

namespace executor{

    using Api::Transaction;

    void execute(const Transaction& tx){
        std::cout << "Executor\n";
        std::cout << "DebugString:"<< tx.DebugString() <<"\n";


        // Temporary - to operate peer service
        if( tx.has_peer() ) {
            peer::Node query_peer(
                    tx.peer().address(),
                    tx.peer().publickey(),
                    tx.peer().trust().value(),
                    tx.peer().trust().isok()
            );
            if( tx.type() == "Add" ) {
                config::PeerServiceConfig::getInstance().addPeer( query_peer );
            } else if( tx.type() == "Remove" ) {
                config::PeerServiceConfig::getInstance().removePeer( query_peer.getPublicKey() );
            } else if( tx.type() == "Update" ) {
                config::PeerServiceConfig::getInstance().updatePeer( query_peer.getPublicKey(), query_peer );
            }
        }
    }

};
