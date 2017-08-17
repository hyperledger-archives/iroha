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

#include "module/irohad/ametsuchi/ametsuchi_mocks.hpp"
#include "module/irohad/network/network_mocks.hpp"
#include "module/irohad/validation/validation_mocks.hpp"

#include <endpoint.pb.h>
#include <queries.pb.h>
#include <atomic>
#include <chrono>
#include <main/server_runner.hpp>
#include <memory>
#include <thread>
#include <torii/command_client.hpp>
#include <torii/command_service.hpp>
#include <torii/processor/query_processor_impl.hpp>
#include <torii_utils/query_client.hpp>

#include "torii/processor/transaction_processor_impl.hpp"

constexpr const char *Ip = "0.0.0.0";
constexpr int Port = 50051;

constexpr size_t TimesToriiBlocking = 5;
constexpr size_t TimesToriiNonBlocking = 5;
constexpr size_t TimesFind = 10;

using ::testing::Return;
using ::testing::A;
using ::testing::_;
using ::testing::AtLeast;

using namespace iroha::network;
using namespace iroha::validation;
using namespace iroha::ametsuchi;

class ToriiServiceTest : public testing::Test {
 public:
  virtual void SetUp() {
    runner = new ServerRunner(std::string(Ip) + ":" + std::to_string(Port));
    th = std::thread([this] {
      // ----------- Command Service --------------
      pcsMock = std::make_shared<MockPeerCommunicationService>();
      statelessValidatorMock = std::make_shared<MockStatelessValidator>();
      wsv_query = std::make_shared<MockWsvQuery>();
      block_query = std::make_shared<MockBlockQuery>();


      auto tx_processor =
          std::make_shared<iroha::torii::TransactionProcessorImpl>(
              pcsMock, statelessValidatorMock);
      auto pb_tx_factory =
          std::make_shared<iroha::model::converters::PbTransactionFactory>();
      auto command_service =
          std::make_unique<torii::CommandService>(pb_tx_factory, tx_processor);

      //----------- Query Service ----------
      auto qpf = std::make_unique<iroha::model::QueryProcessingFactory>(
          wsv_query, block_query);

      auto qpi = std::make_shared<iroha::torii::QueryProcessorImpl>(
          std::move(qpf), statelessValidatorMock);

      auto pb_query_factory =
          std::make_shared<iroha::model::converters::PbQueryFactory>();
      auto pb_query_resp_factory =
          std::make_shared<iroha::model::converters::PbQueryResponseFactory>();

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

  std::shared_ptr<MockWsvQuery> wsv_query;
  std::shared_ptr<MockBlockQuery> block_query;

  std::shared_ptr<MockPeerCommunicationService> pcsMock;
  std::shared_ptr<MockStatelessValidator> statelessValidatorMock;
};

TEST_F(ToriiServiceTest, ToriiWhenBlocking) {
  EXPECT_CALL(*statelessValidatorMock,
              validate(A<const iroha::model::Transaction &>()))
      .Times(TimesToriiBlocking)
      .WillRepeatedly(Return(true));

  EXPECT_CALL(*pcsMock, propagate_transaction(_)).Times(AtLeast(1));

  for (size_t i = 0; i < TimesToriiBlocking; ++i) {
    auto new_tx = iroha::protocol::Transaction();
    auto meta = new_tx.mutable_meta();
    meta->set_tx_counter(i);
    meta->set_creator_account_id("accountA");

    auto stat = torii::CommandSyncClient(Ip, Port).Torii(new_tx);
    ASSERT_TRUE(stat.ok());
  }
}

TEST_F(ToriiServiceTest, ToriiWhenBlockingInvalid) {
  EXPECT_CALL(*statelessValidatorMock,
              validate(A<const iroha::model::Transaction &>()))
      .Times(TimesToriiBlocking)
      .WillRepeatedly(Return(false));

  for (size_t i = 0; i < TimesToriiBlocking; ++i) {
    auto new_tx = iroha::protocol::Transaction();
    auto meta = new_tx.mutable_meta();
    meta->set_tx_counter(i);
    meta->set_creator_account_id("accountA");
    auto stat = torii::CommandSyncClient(Ip, Port).Torii(new_tx);
    ASSERT_TRUE(stat.ok());
  }
}

TEST_F(ToriiServiceTest, ToriiWhenNonBlocking) {
  torii::CommandAsyncClient client(Ip, Port);
  std::atomic_int count{0};

  EXPECT_CALL(*statelessValidatorMock,
              validate(A<const iroha::model::Transaction &>()))
      .Times(TimesToriiNonBlocking)
      .WillRepeatedly(Return(true));

  EXPECT_CALL(*pcsMock, propagate_transaction(_)).Times(AtLeast(1));

  for (size_t i = 0; i < TimesToriiNonBlocking; ++i) {
    auto new_tx = iroha::protocol::Transaction();
    auto meta = new_tx.mutable_meta();
    meta->set_tx_counter(i);
    meta->set_creator_account_id("accountA");

    auto stat =
        client.Torii(new_tx, [&count](auto response) {
          count++;
        });
  }

  while (count < (int)TimesToriiNonBlocking)
    ;
  ASSERT_EQ(count, TimesToriiNonBlocking);
}
