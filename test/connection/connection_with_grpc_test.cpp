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

#include <string>
#include <iostream>
#include <unordered_map>

#include <gtest/gtest.h>

#include <consensus/connection/connection.hpp>
#include <consensus/consensus_event.hpp>
#include <service/peer_service.hpp>
#include <infra/protobuf/api.grpc.pb.h>
#include <infra/config/peer_service_with_json.hpp>

using Api::ConsensusEvent;

TEST(ConnectionWithGrpc, sample){
    logger::setLogLevel(logger::LogLevel::DEBUG);

    connection::initialize_peer();

    auto server = []() {
        connection::iroha::Sumeragi::Verify::receive([](const std::string &from, ConsensusEvent &event) {
            std::cout << " receive : sig size:" << event.eventsignatures_size() << "\n";
            std::cout << " receive : name:" << event.transaction().asset().name() << "\n";
            std::cout << " type:" << event.transaction().type() << "\n";
        });
        connection::run();
    };

    connection::iroha::Sumeragi::Verify::addSubscriber(
        config::PeerServiceConfig::getInstance().getMyIp()
    );

    connection::finish();
}
