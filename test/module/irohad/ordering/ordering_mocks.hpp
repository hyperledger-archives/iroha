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

#ifndef IROHA_ORDERING_MOCKS_HPP
#define IROHA_ORDERING_MOCKS_HPP

#include <gmock/gmock.h>
#include "network/ordering_gate.hpp"
#include "ordering.grpc.pb.h"

class FakeOrderingGate : public iroha::network::OrderingGate,
                         public iroha::ordering::proto::OrderingGate::Service {
 public:
  MOCK_METHOD1(propagate_transaction, void(const iroha::model::Transaction&));
  MOCK_METHOD0(on_proposal, rxcpp::observable<iroha::model::Proposal>());
  MOCK_METHOD3(SendProposal,
               grpc::Status(::grpc::ServerContext*,
                            const iroha::ordering::proto::Proposal*,
                            ::google::protobuf::Empty*));
};

class FakeOrderingService
    : public iroha::ordering::proto::OrderingService::Service {
 public:
  MOCK_METHOD3(SendTransaction,
               ::grpc::Status(::grpc::ServerContext*,
                              const iroha::protocol::Transaction*,
                              ::google::protobuf::Empty*));
};

class OrderingTest : public ::testing::Test {
 public:
  OrderingTest() {
    address = "0.0.0.0:50051";
    peer.address = address;
    gate = std::make_shared<FakeOrderingGate>();
    service = std::make_shared<FakeOrderingService>();
  }

  void SetUp() override { start(); }

  void TearDown() override { shutdown(); }

  virtual void start() {
    std::mutex mtx;
    std::condition_variable cv;
    thread = std::thread([&cv, this] {
      grpc::ServerBuilder builder;
      int port = 0;
      builder.AddListeningPort(address, grpc::InsecureServerCredentials(),
                               &port);
      builder.RegisterService(service.get());
      builder.RegisterService(gate.get());
      server = builder.BuildAndStart();
      ASSERT_NE(port, 0);
      ASSERT_TRUE(server);
      cv.notify_one();
      server->Wait();
    });

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait_for(lock, std::chrono::seconds(1));
  }

  virtual void shutdown() {
    server->Shutdown();
    if (thread.joinable()) {
      thread.join();
    }
  }

  virtual ~OrderingTest() = default;

  std::string address;
  iroha::model::Peer peer;
  std::unique_ptr<grpc::Server> server;
  std::shared_ptr<iroha::ordering::proto::OrderingGate::Service> gate;
  std::shared_ptr<iroha::ordering::proto::OrderingService::Service> service;
  std::thread thread;
};

#endif  // IROHA_ORDERING_MOCKS_HPP
