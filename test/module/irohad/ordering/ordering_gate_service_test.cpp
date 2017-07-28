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

#include <grpc++/grpc++.h>
#include <gtest/gtest.h>
#include "ordering/impl/ordering_gate_impl.hpp"
#include "ordering/impl/ordering_service_impl.hpp"

using namespace iroha::ordering;
using namespace iroha::model;
using namespace iroha::network;

TEST(OrderingGateServiceTest, SampleTest) {
  auto loop = uvw::Loop::getDefault();

  auto address = "0.0.0.0:50051";
  Peer peer;
  peer.address = address;

  auto gate = std::make_shared<OrderingGateImpl>(peer.address);
  auto service = std::make_shared<OrderingServiceImpl>(std::vector<Peer>{peer},
                                                       5, 1600, loop);
  std::unique_ptr<grpc::Server> server;

  std::vector<Proposal> proposals;

  gate->on_proposal().subscribe(
      [&proposals](auto proposal) { proposals.push_back(proposal); });

  std::mutex mtx;
  std::condition_variable cv;

  auto s_thread = std::thread([&cv, address, &service, &server, &gate] {
    grpc::ServerBuilder builder;
    int port = 0;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials(), &port);
    builder.RegisterService(service.get());
    builder.RegisterService(gate.get());
    server = builder.BuildAndStart();
    ASSERT_NE(port, 0);
    ASSERT_TRUE(server);
    cv.notify_one();
    server->Wait();
  });

  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock);

  auto l_thread = std::thread([&loop] { loop->run(); });

  for (size_t i = 0; i < 10; ++i) {
    gate->propagate_transaction(Transaction());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ASSERT_EQ(proposals.size(), 3);
  ASSERT_EQ(proposals.at(0).transactions.size(), 4);
  ASSERT_EQ(proposals.at(1).transactions.size(), 3);
  ASSERT_EQ(proposals.at(2).transactions.size(), 3);

  loop->stop();
  server->Shutdown();
  if (l_thread.joinable()) {
    l_thread.join();
  }
  if (s_thread.joinable()) {
    s_thread.join();
  }
}