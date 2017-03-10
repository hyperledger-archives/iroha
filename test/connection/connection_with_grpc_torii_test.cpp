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
#include <infra/config/peer_service_with_json.hpp>

#include <transaction_builder/transaction_builder.hpp>

using Api::Transaction;
using txbuilder::TransactionBuilder;
using type_signatures::Add;
using type_signatures::Peer;

TEST(ConnectionWithGrpcTorii, Transaction_Add_Peer){
    logger::setLogLevel(logger::LogLevel::Debug);

    connection::initialize_peer();

    auto server = []() {
        connection::iroha::Sumeragi::Torii::receive([](const std::string &from, Transaction &transaction) {
            ASSERT_STREQ( transaction.senderpubkey().c_str(),              "sate");
            ASSERT_STREQ( transaction.peer().publickey().c_str(),          "light");
            ASSERT_STREQ( transaction.peer().address().c_str(),            "test_ip");
            ASSERT_TRUE( transaction.peer().trust().value() == 1.0 );
        });
        connection::run();
    };

    std::thread server_thread(server);

    auto tx = TransactionBuilder<Add<Peer>>()
            .setSenderPublicKey("sate")
            .setPeer( txbuilder::createPeer( "light", "test_ip", txbuilder::createTrust( 1.0, true ) ) )
            .build();

    connection::iroha::PeerService::Sumeragi::send(
            config::PeerServiceConfig::getInstance().getMyIp(),
            tx
    );

    server_thread.detach();
    connection::finish();

}