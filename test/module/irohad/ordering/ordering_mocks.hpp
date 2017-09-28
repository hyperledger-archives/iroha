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

#include <grpc++/grpc++.h>
#include "module/irohad/network/network_mocks.hpp"
#include "network/ordering_gate.hpp"
#include "ordering.grpc.pb.h"
#include "ordering/impl/ordering_service_impl.hpp"

namespace iroha {
  namespace ordering {
    class MockOrderingGate : public network::MockOrderingGate,
                             public proto::OrderingGateTransportGrpc::Service {
     public:
      MOCK_METHOD3(onProposal, grpc::Status(::grpc::ServerContext *,
                                            const proto::Proposal *,
                                            ::google::protobuf::Empty *));
    };

    class MockOrderingService
        : public proto::OrderingServiceTransportGrpc::Service {
     public:
      MOCK_METHOD3(onTransaction, ::grpc::Status(::grpc::ServerContext *,
                                                 const protocol::Transaction *,
                                                 ::google::protobuf::Empty *));
    };

    class OrderingTest : public ::testing::Test {
     public:
      OrderingTest() {
        address = "0.0.0.0:50051";
        peer.address = address;
        gate_transport_service = std::make_shared<MockOrderingGate>();
        service_transport_service = std::make_shared<MockOrderingService>();
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
          builder.RegisterService(service_transport_service.get());
          builder.RegisterService(gate_transport_service.get());
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
      model::Peer peer;
      std::unique_ptr<grpc::Server> server;
      std::shared_ptr<proto::OrderingGateTransportGrpc::Service>
          gate_transport_service;
      std::shared_ptr<proto::OrderingServiceTransportGrpc::Service>
          service_transport_service;

      std::shared_ptr<iroha::network::OrderingGate> gate;
      std::shared_ptr<iroha::ordering::OrderingServiceImpl> service;
      std::thread thread;
    };
  }  // namespace ordering
}  // namespace iroha

#endif  // IROHA_ORDERING_MOCKS_HPP
