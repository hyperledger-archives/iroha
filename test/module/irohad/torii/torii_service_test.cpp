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

#include <endpoint.pb.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <main/server_runner.hpp>
#include <memory>
#include <thread>
#include <torii/command_client.hpp>
#include <torii/command_service.hpp>
#include <torii/torii_service_handler.hpp>
#include <torii_utils/query_client.hpp>

constexpr const char* Ip = "0.0.0.0";
constexpr int Port = 50051;

constexpr size_t TimesToriiBlocking = 100;
constexpr size_t TimesToriiNonBlocking = 3;
constexpr size_t TimesFind = 100;

using ::testing::Return;

class TransactionProcessorMock : public iroha::torii::TransactionProcessor {
 public:
  MOCK_METHOD1(transaction_handle, void(iroha::model::Transaction&));

  MOCK_METHOD0(
      transaction_notifier,
      rxcpp::observable<std::shared_ptr<iroha::model::TransactionResponse>>());
};

class ToriiServiceTest : public testing::Test {
 public:
  virtual void SetUp() {
    runner = new ServerRunner(Ip, Port);
    th = std::thread([runner = runner] {
      TransactionProcessorMock tx_processor;
      iroha::model::converters::PbTransactionFactory pb_factory;

      EXPECT_CALL(tx_processor, transaction_notifier())
          .WillRepeatedly(
              Return(rxcpp::observable<>::create<
                     std::shared_ptr<iroha::model::TransactionResponse>>([](
                  auto s) {
                iroha::model::TransactionStatelessResponse response;
                response.passed = true;
                s.on_next(std::make_shared<iroha::model::TransactionResponse>(
                    response));
                s.on_completed();
              })));

      auto command_service = std::make_unique<torii::CommandService>(
          torii::CommandService(pb_factory, tx_processor));
      runner->run(std::move(command_service));
    });

    runner->waitForServersReady();
  }

  virtual void TearDown() {
    runner->shutdown();
    delete runner;
    th.join();
  }

  ServerRunner* runner;
  std::thread th;
};

TEST_F(ToriiServiceTest, ToriiWhenBlocking) {
  for (size_t i = 0; i < TimesToriiBlocking; ++i) {
    std::cout << i << std::endl;
    iroha::protocol::ToriiResponse response;
    auto new_tx = iroha::protocol::Transaction();
    auto meta = new_tx.mutable_meta();
    meta->set_tx_counter(i);
    auto stat = torii::CommandSyncClient(Ip, Port).Torii(
        new_tx, response);
    ASSERT_TRUE(stat.ok());
    ASSERT_EQ(response.validation(),
              iroha::protocol::STATELESS_VALIDATION_SUCCESS);
    // std::cout << response.message() << std::endl;
    // std::cout << "\n";
  }
}
/*
TEST_F(ToriiServiceTest, ToriiWhenNonBlocking) {
  torii::CommandAsyncClient client(Ip, Port);
  std::atomic_int count{0};

  for (size_t i = 0; i < TimesToriiNonBlocking; ++i) {
    std::cout << i << std::endl;
    auto stat = client.Torii(iroha::protocol::Transaction{},
                             [&count](iroha::protocol::ToriiResponse response) {
                               ASSERT_EQ(response.code(),
                                         iroha::protocol::ResponseCode::OK);
                               std::cout << "Async response\n";
                               count++;
                             });
  }

  while (count < (int)TimesToriiNonBlocking)
    ;
  ASSERT_EQ(count, TimesToriiNonBlocking);
}

TEST_F(ToriiServiceTest, FindWhereQueryServiceSync) {
  iroha::protocol::QueryResponse response;
  auto stat = torii_utils::QuerySyncClient(Ip, Port).Find(
      iroha::protocol::Query{}, response);
  ASSERT_TRUE(stat.ok());
}

TEST_F(ToriiServiceTest, FindManyTimesWhereQueryServiceSync) {
  for (size_t i = 0; i < TimesFind; ++i) {
    iroha::protocol::QueryResponse response;
    auto stat = torii_utils::QuerySyncClient(Ip, Port).Find(
        iroha::protocol::Query{}, response);
    ASSERT_TRUE(stat.ok());
  }
}

TEST_F(ToriiServiceTest, MixRPCWhereCommandAndQueryService) {
  torii::CommandAsyncClient client(Ip, Port);

  for (size_t i = 0; i < TimesFind; ++i) {
    iroha::protocol::QueryResponse qresp;
    grpc::Status stat;
    stat = torii_utils::QuerySyncClient(Ip, Port).Find(iroha::protocol::Query{},
                                                       qresp);
    ASSERT_TRUE(stat.ok());
    client.Torii(iroha::protocol::Transaction{},
                 [](iroha::protocol::ToriiResponse response) {
                   ASSERT_EQ(response.code(),
                             iroha::protocol::ResponseCode::OK);
                   std::cout << "Async response\n";
                 });
    iroha::protocol::ToriiResponse response;
    stat = torii::CommandSyncClient(Ip, Port).Torii(
        iroha::protocol::Transaction{}, response);
    ASSERT_TRUE(stat.ok());
    std::cout << "Sync Response\n";
  }
}
*/