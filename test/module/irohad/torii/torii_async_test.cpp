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
#include <torii/command_service_handler.hpp>
#include <torii/command_service.hpp>
#include <torii/command_client.hpp>
#include <endpoint.pb.h>
#include <thread>
#include <memory>

constexpr const char* Ip = "0.0.0.0";
constexpr int Port = 50051;

class ToriiAsyncTest : public testing::Test {
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

TEST_F(ToriiAsyncTest, ToriiWhenBlocking) {
  EXPECT_GT(static_cast<int>(iroha::protocol::ResponseCode::OK), 0); // to guarantee ASSERT_EQ works TODO(motxx): More reasonable way.
  for (int i = 0; i < 100; i++) {
    std::cout << i << std::endl;
    auto response = torii::CommandSyncClient(Ip, Port)
      .Torii(iroha::protocol::Transaction {});
    ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
    std::cout << "Sync Response\n";
  }
}

TEST_F(ToriiAsyncTest, ToriiWhenNonBlocking) {
  EXPECT_GT(static_cast<int>(iroha::protocol::ResponseCode::OK), 0);

  torii::CommandAsyncClient client(Ip, Port);

  for (int i = 0; i < 100; i++) {
    std::cout << i << std::endl;
    client.Torii(iroha::protocol::Transaction {},
                 [](iroha::protocol::ToriiResponse response){
                   ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
                   std::cout << "Async response\n";
                 });
  }
}
