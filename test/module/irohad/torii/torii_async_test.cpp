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

constexpr const char* Ip = "0.0.0.0";
constexpr int Port = 50051;

ServerRunner runner {Ip, Port};
std::thread th;

class ToriiAsyncTest : public testing::Test {
public:
  virtual void SetUp() {
    th = std::thread([]{
      runner.run();
    });

    runner.waitForServersReady();
  }

  virtual void TearDown() {
    runner.shutdown();
    th.join();
  }
};

TEST_F(ToriiAsyncTest, ToriiBlocking) {
  EXPECT_GT(static_cast<int>(iroha::protocol::ResponseCode::OK), 0); // to guarantee ASSERT_EQ works TODO(motxx): More reasonable way.
  for (int i = 0; i < 100; i++) {
    std::cout << i << std::endl;
    auto response = torii::CommandClient(Ip, Port)
      .ToriiBlocking(iroha::protocol::Transaction {});
    ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
  }
}

/*
 * TODO(motxx): We can't use CommandClient::ToriiNonBlocking() for now. gRPC causes the error
 * E0714 04:24:40.045388600    4346 sync_posix.c:60]            assertion failed: pthread_mutex_lock(mu) == 0
 */
/*
TEST_F(ToriiAsyncTest, ToriiNonBlocking) {
  EXPECT_GT(static_cast<int>(iroha::protocol::ResponseCode::OK), 0);
  for (int i = 0; i < 100; i++) {
    std::cout << i << std::endl;
    torii::CommandClient(Ip, Port)
      .ToriiNonBlocking(iroha::protocol::Transaction {},
                        [](iroha::protocol::ToriiResponse response){
                          ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
                          std::cout << "Response validated\n"; // for checking really asynced.
                        });
  }
}
*/