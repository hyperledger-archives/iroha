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
#include <consensus/connection/service.hpp>
#include <consensus/connection/client.hpp>
#include <main/server_runner.hpp>
#include <thread>
#include <endpoint.grpc.pb.h>

using namespace consensus::connection;
using iroha::protocol::Block;

class ConsensusConnectionTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    std::vector<grpc::Service*> services {
      &service_
    };
    serverRunner_ = std::make_unique<ServerRunner>("0.0.0.0", 50051, services);
    running_ = false;
  }

  virtual void TearDown() {
    if (running_) {
      serverRunner_->shutDown();
      serverThread_.join();
    }
  }

  void RunServer() {
    serverThread_ = std::thread(&ServerRunner::run, serverRunner_.get());
    serverRunner_->waitForServersReady();
    running_ = true;
  }

 private:
  bool running_;
  iroha::protocol::SumeragiService::Service service_;
  std::unique_ptr<ServerRunner> serverRunner_;
  std::thread serverThread_;
};

/**
 * Note: Async connection is WIP.
 *       Temporarily, we tests sync connection.
 */
TEST_F(ConsensusConnectionTest, FailConnectionWhenNotStandingServer) {
  Block block;
  block.mutable_header()->set_merkle_root("merkle");
  auto response = unicast(block, "0.0.0.0");
  ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::FAIL);
}

TEST_F(ConsensusConnectionTest, SuccessConnectionWhenStandingServer) {
  RunServer();
  Block block;
  block.mutable_header()->set_merkle_root("merkle");
  auto response = unicast(block, "0.0.0.0");
  ASSERT_EQ(response.code(), iroha::protocol::ResponseCode::OK);
}
