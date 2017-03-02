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
#include <thread>
#include <gtest/gtest.h>

#include <consensus/connection/connection.hpp>
#include <consensus/consensus_event.hpp>
#include <service/peer_service.hpp>
#include <infra/protobuf/api.grpc.pb.h>
#include <infra/config/peer_service_with_json.hpp>

#include <transaction_builder/transaction_builder.hpp>

using Api::ConsensusEvent;

using txbuilder::TransactionBuilder;
using type_signatures::Add;
using type_signatures::Domain;
using type_signatures::Account;
using type_signatures::Asset;
using type_signatures::SimpleAsset;
using type_signatures::Peer;


TEST(ConnectionWithGrpc, Transaction_Add_Domain){
    logger::setLogLevel(logger::LogLevel::Debug);

    connection::initialize_peer();

    auto server = []() {
        connection::iroha::Sumeragi::Verify::receive([](const std::string &from, ConsensusEvent &event) {
            std::cout << event.transaction().DebugString() << std::endl;
            ASSERT_STREQ( event.transaction().senderpubkey().c_str(),              "karin");
            ASSERT_STREQ( event.transaction().domain().name().c_str(),              "name");
            ASSERT_STREQ( event.transaction().domain().ownerpublickey().c_str(), "pubkey1");
        });
        connection::run();
    };

    std::thread server_thread(server);

    connection::iroha::Sumeragi::Verify::addSubscriber(
        config::PeerServiceConfig::getInstance().getMyIp()
    );

    Api::Domain domain;
    domain.set_ownerpublickey("pubkey1");
    domain.set_name("name");
    auto tx = TransactionBuilder<Add<Domain>>()
            .setSenderPublicKey("karin")
            .setDomain(domain)
            .build();

    Api::ConsensusEvent sampleEvent;
    sampleEvent.mutable_transaction()->CopyFrom(tx);

    connection::iroha::Sumeragi::Verify::send(
        config::PeerServiceConfig::getInstance().getMyIp(),
        sampleEvent
    );

    server_thread.detach();
    connection::finish();
}
