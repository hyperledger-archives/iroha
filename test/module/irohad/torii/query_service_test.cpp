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
#include <endpoint.grpc.pb.h>
#include <thread>
#include <memory>
#include <chrono>
#include <atomic>

constexpr const char* Ip = "0.0.0.0";
constexpr int Port = 50051;

class QueryServiceTest : public testing::Test {
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

class QueryClient {
public:
  QueryClient(const std::string& ip, int port) {
    auto channel = grpc::CreateChannel(ip + ":" + std::to_string(port), grpc::InsecureChannelCredentials());
    stub_ = iroha::protocol::QueryService::NewStub(channel);
  }

  bool Find(iroha::protocol::Query const& request) {
    iroha::protocol::QueryResponse response;
    auto status = stub_->Find(&context_, request, &response);
    return status.ok();
  }

private:
  std::unique_ptr<iroha::protocol::QueryService::Stub> stub_;
  grpc::ClientContext context_;
};

TEST_F(QueryServiceTest, FindWhereQueryServiceSync) {
  QueryClient client(Ip, Port);
  iroha::protocol::Query request;
  ASSERT_TRUE(client.Find(request));
}
