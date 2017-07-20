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

#include <gtest/gtest.h>
#include <main/server_runner.hpp>
#include <torii/torii_service_handler.hpp>
#include <torii/command_service.hpp>
#include <torii/command_client.hpp>
#include <torii_utils/query_client.hpp>
#include <endpoint.pb.h>
#include <thread>
#include <memory>
#include <chrono>
#include <atomic>

constexpr const char* Ip = "0.0.0.0";
constexpr int Port = 50051;

constexpr size_t TimesToriiBlocking = 100;
constexpr size_t TimesToriiNonBlocking = 3;
constexpr size_t TimesFind = 100;

class ToriiServiceTest : public testing::Test {
public:
  virtual void SetUp() {
    runner = new ServerRunner(Ip, Port);
    th = std::thread([runner = runner]{
      runner->run();
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
  EXPECT_GT(static_cast<int>(iroha::protocol::ResponseCode::OK), 0); // to guarantee ASSERT_EQ works TODO(motxx): More reasonable way.

  for (size_t i = 0; i < TimesToriiBlocking; ++i) {
    std::cout << i << std::endl;
    auto response = torii::CommandSyncClient(Ip, Port)
      .Torii(iroha::protocol::Transaction {});
    ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
    std::cout << "Sync Response\n";
  }
}

TEST_F(ToriiServiceTest, ToriiWhenNonBlocking) {
  EXPECT_GT(static_cast<int>(iroha::protocol::ResponseCode::OK), 0);

  torii::CommandAsyncClient client(Ip, Port);
  std::atomic_int count {0};

  for (size_t i = 0; i < TimesToriiNonBlocking; ++i) {
    std::cout << i << std::endl;
    client.Torii(iroha::protocol::Transaction {},
                 [&count](iroha::protocol::ToriiResponse response){
                   ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
                   std::cout << "Async response\n";
                   count++;
                 });
  }

  while (count < (int)TimesToriiNonBlocking);
  ASSERT_EQ(count, TimesToriiNonBlocking);
}

TEST_F(ToriiServiceTest, FindWhereQueryServiceSync) {
  EXPECT_GT(iroha::protocol::ResponseCode::OK, 0);

  auto response = torii_utils::QuerySyncClient(Ip, Port).Find(iroha::protocol::Query {});
  ASSERT_TRUE(response.code() == iroha::protocol::ResponseCode::OK);
  ASSERT_STRNE(response.message().c_str(), torii_utils::FailureMessage);
}

TEST_F(ToriiServiceTest, FindManyTimesWhereQueryServiceSync) {
  EXPECT_GT(iroha::protocol::ResponseCode::OK, 0);

  for (size_t i = 0; i < TimesFind; ++i) {
    auto response = torii_utils::QuerySyncClient(Ip, Port).Find(iroha::protocol::Query {});
    ASSERT_TRUE(response.code() == iroha::protocol::ResponseCode::OK);
    ASSERT_STRNE(response.message().c_str(), torii_utils::FailureMessage);
  }
}

TEST_F(ToriiServiceTest, MixRPCWhereCommandAndQueryService) {
  EXPECT_GT(iroha::protocol::ResponseCode::OK, 0);
  torii::CommandAsyncClient client(Ip, Port);

  for (size_t i = 0; i < TimesFind; ++i) {
    auto response = torii_utils::QuerySyncClient(Ip, Port).Find(iroha::protocol::Query {});
    ASSERT_TRUE(response.code() == iroha::protocol::ResponseCode::OK);
    ASSERT_STRNE(response.message().c_str(), torii_utils::FailureMessage);
    client.Torii(iroha::protocol::Transaction {},
      [](iroha::protocol::ToriiResponse response){
        ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
        std::cout << "Async response\n";
      });
    auto toriiSyncResponse = torii::CommandSyncClient(Ip, Port)
      .Torii(iroha::protocol::Transaction {});
    ASSERT_EQ(toriiSyncResponse.code(), iroha::protocol::ResponseCode::OK);
    std::cout << "Sync Response\n";
  }
}
