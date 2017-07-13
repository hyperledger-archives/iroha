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

const std::string Ip = "0.0.0.0";
const int Port = 50051;

TEST(ToriiAsyncTest, GetToriiResponseWhenSendingTx) {
  ServerRunner runner(Ip, Port);
  std::thread th([&runner]{
    runner.run();
  });

  runner.waitForServersReady();

  for (int i = 0; i < 100; i++) {
    std::cout << i << std::endl;
    auto response = torii::sendTransaction(iroha::protocol::Transaction {}, Ip, Port);
    ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
    ASSERT_STREQ(response.message().c_str(), "Torii async response");
  }

  // TODO(motxx): Segmentation fault occurs because an event doesn't executed that causes completion queue to be shut down.
  runner.shutdown();
  th.join();
}
