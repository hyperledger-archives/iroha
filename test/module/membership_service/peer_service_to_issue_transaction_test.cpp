/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Takumi Yamashita on 2017/05/02.
//

#include <gtest/gtest.h>
#include <service/flatbuffer_service.h>
#include <transaction_generated.h>
#include <membership_service/peer_service.hpp>
#include <service/connection.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

using Transaction = iroha::Transaction;

class peer_service_to_issue_transaction_test : public ::testing::Test {
 protected:
  void serverToriiReceive() {
    connection::iroha::SumeragiImpl::Torii::receive([](
        const std::string& from, flatbuffers::unique_ptr_t&& transaction) {
      auto tx = flatbuffers::GetRoot<::iroha::Transaction>(transaction.get());
      std::cout << "Command type: " << (int)tx->command_type() << std::endl;

      if (tx->command_type() == iroha::Command::PeerAdd) {
        std::cout << "Command Add!" << std::endl;
        auto peer_add = tx->command_as_PeerAdd();
        auto peer = peer_add->peer_nested_root();
        ASSERT_TRUE(peer->ip()->str() == "new_ip");
        ASSERT_TRUE(peer->publicKey()->str() == "new_pubkey");
        ASSERT_TRUE(peer->ledger_name()->str() == "ledger");
        ASSERT_TRUE(peer->trust() == 100.0);
        ASSERT_TRUE(peer->active() == false);
        ASSERT_TRUE(peer->join_ledger() == true);
        std::cout << peer->ip()->str() << std::endl;
        std::cout << peer->publicKey()->str() << std::endl;
        std::cout << peer->ledger_name()->str() << std::endl;
        std::cout << peer->trust() << std::endl;
        std::cout << peer->active() << std::endl;
        std::cout << peer->join_ledger() << std::endl;
      }
      /*
       *
      ASSERT_EQ(transaction.senderpubkey(), toriiSenderPubKey);
      ASSERT_EQ(transaction.peer().publickey(), toriiPeerPubKey);
      ASSERT_EQ(transaction.peer().address(), toriiPeerAddress);
      ASSERT_TRUE(transaction.peer().trust().value() == 1.0);
       */
    });
    connection::run();
  }
  std::thread server_thread_torii;

  static void SetUpTestCase() { connection::initialize_peer(); }

  static void TearDownTestCase() { connection::finish(); }

  virtual void SetUp() {
    server_thread_torii = std::thread(
        &peer_service_to_issue_transaction_test::serverToriiReceive, this);
  }

  virtual void TearDown() { server_thread_torii.detach(); }
};


TEST_F(peer_service_to_issue_transaction_test, PeerAddTest) {
  ::peer::myself::activate();
  const auto peer = ::peer::Node("new_ip", "new_pubkey", "ledger");
  ::peer::transaction::isssue::add(::peer::myself::getIp(), peer);
}