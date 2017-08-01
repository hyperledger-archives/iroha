/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
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

#include "../../../iroha-cli/client.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../irohad/torii/mock_classes.hpp"
#include "main/server_runner.hpp"
#include "torii/processor/query_processor_impl.hpp"
#include "torii/processor/transaction_processor_impl.hpp"

constexpr const char *Ip = "0.0.0.0";
constexpr int Port = 50051;

using ::testing::Return;
using ::testing::A;
using ::testing::_;
using ::testing::AtLeast;

class ClientTest : public testing::Test {
 public:
  virtual void SetUp() {
    // Create public key file

    // Run a server
    runner = new ServerRunner(Ip, Port);
    th = std::thread([ this] {
      // ----------- Command Service --------------
      auto tx_processor =
          iroha::torii::TransactionProcessorImpl(pcsMock, svMock);
      iroha::model::converters::PbTransactionFactory pb_tx_factory;
      auto command_service =
          std::make_unique<torii::CommandService>(pb_tx_factory, tx_processor);

      //----------- Query Service ----------
      iroha::model::QueryProcessingFactory qpf(wsv_query, block_query);

      iroha::torii::QueryProcessorImpl qpi(qpf, svMock);

      iroha::model::converters::PbQueryFactory pb_query_factory;
      iroha::model::converters::PbQueryResponseFactory pb_query_resp_factory;

      auto query_service = std::make_unique<torii::QueryService>(
          pb_query_factory, pb_query_resp_factory, qpi);

      //----------- Server run ----------------
      runner->run(std::move(command_service), std::move(query_service));
    });

    runner->waitForServersReady();
  }

  virtual void TearDown() {
    runner->shutdown();
    delete runner;
    th.join();
  }

  ServerRunner *runner;
  std::thread th;
  PCSMock pcsMock;
  SVMock svMock;
  WsvQueryMock wsv_query;
  BlockQueryMock block_query;
};

TEST_F(ClientTest, SendTxWhenValid) {
  std::string account_name = "test";
  iroha_cli::CliClient::create_account(account_name);
  iroha_cli::CliClient client(Ip, Port);
  EXPECT_CALL(svMock, validate(A<const iroha::model::Transaction &>()))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(pcsMock,propagate_transaction(_)).Times(1);

  auto json_tx ="{\n"
      "  \"creator_account_id\": \"test\", \n"
      "  \"tx_counter\": 0,\n"
      "  \"commands\":[{\n"
      "  \"command_type\": \"AddPeer\",\n"
      "    \"address\": \"localhost\",\n"
      "    \"peer_key\": \"2323232323232323232323232323232323232323232323232323232323232323\"\n"
      "  }]\n"
      "}";
  std::cout << "Sending  json transaction to Iroha" << std::endl;
  auto status = client.sendTx(json_tx);
  ASSERT_EQ(status, iroha_cli::CliClient::OK);
  // Remove private and public key
  std::remove((account_name+".pub").c_str());
  std::remove((account_name+".priv").c_str());
}

TEST_F(ClientTest, SendTxWhenInvalidJson) {
  std::string account_name = "test";
  iroha_cli::CliClient::create_account(account_name);
  iroha_cli::CliClient client(Ip, Port);
  EXPECT_CALL(svMock, validate(A<const iroha::model::Transaction &>()))
      .WillRepeatedly(Return(true));
  // Json with no Transaction
  auto json_tx ="{\n"
      "  \"creator_account_id\": \"test\", \n"
      "  \"commands\":[{\n"
      "  \"command_type\": \"AddPeer\",\n"
      "    \"address\": \"localhost\",\n"
      "    \"peer_key\": \"2323232323232323232323232323232323232323232323232323232323232323\"\n"
      "  }]\n"
      "}";
  std::cout << "Sending  json transaction to Iroha" << std::endl;
  ASSERT_EQ(client.sendTx(json_tx), iroha_cli::CliClient::WRONG_FORMAT);
  // Remove private and public key
  std::remove((account_name+".pub").c_str());
  std::remove((account_name+".priv").c_str());
}


TEST_F(ClientTest, SendTxWhenStatelessInvalid) {
  std::string account_name = "test";
  iroha_cli::CliClient::create_account(account_name);
  iroha_cli::CliClient client(Ip, Port);
  EXPECT_CALL(svMock, validate(A<const iroha::model::Transaction &>()))
      .WillRepeatedly(Return(false));
  // Json with no Transaction
  auto json_tx ="{\n"
      "  \"creator_account_id\": \"test\", \n"
      "  \"tx_counter\": 0,\n"
      "  \"commands\":[{\n"
      "  \"command_type\": \"AddPeer\",\n"
      "    \"address\": \"localhost\",\n"
      "    \"peer_key\": \"2323232323232323232323232323232323232323232323232323232323232323\"\n"
      "  }]\n"
      "}";
  std::cout << "Sending  json transaction to Iroha" << std::endl;
  ASSERT_EQ(client.sendTx(json_tx), iroha_cli::CliClient::NOT_VALID);
  // Remove private and public key
  std::remove((account_name+".pub").c_str());
  std::remove((account_name+".priv").c_str());
}



TEST_F(ClientTest, SendTxWhenNoKeys) {
  std::string account_name = "test";
  // Client without public, private keys
  iroha_cli::CliClient client(Ip, Port);
  EXPECT_CALL(svMock, validate(A<const iroha::model::Transaction &>()))
      .WillRepeatedly(Return(true));
  // Json with no Transaction
  auto json_tx ="{\n"
      "  \"creator_account_id\": \"test\", \n"
      "  \"tx_counter\": 0,\n"
      "  \"commands\":[{\n"
      "  \"command_type\": \"AddPeer\",\n"
      "    \"address\": \"localhost\",\n"
      "    \"peer_key\": \"2323232323232323232323232323232323232323232323232323232323232323\"\n"
      "  }]\n"
      "}";
  std::cout << "Sending  json transaction to Iroha" << std::endl;
  ASSERT_EQ(client.sendTx(json_tx), iroha_cli::CliClient::NO_KEYS);
}