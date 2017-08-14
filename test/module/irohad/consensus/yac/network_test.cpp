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

#include "module/irohad/consensus/yac/yac_mocks.hpp"

#include <grpc++/grpc++.h>
#include "consensus/yac/impl/network_impl.hpp"

using namespace iroha::consensus::yac;
using iroha::model::Peer;

using ::testing::_;
using ::testing::InvokeWithoutArgs;

TEST(NetworkTest, MessageHandledWhenMessageSent) {
  auto notifications = std::make_shared<MockYacNetworkNotifications>();

  auto peer = mk_peer("0.0.0.0:50051");
  std::vector<Peer> peers = {peer};
  std::shared_ptr<NetworkImpl> network =
      std::make_shared<NetworkImpl>(peer.address, peers);

  VoteMessage message;
  message.hash.proposal_hash = "proposal";
  message.hash.block_hash = "block";

  std::mutex mtx;
  std::condition_variable cv;
  ON_CALL(*notifications, on_vote(peer, message))
      .WillByDefault(
          InvokeWithoutArgs(&cv, &std::condition_variable::notify_one));

  EXPECT_CALL(*notifications, on_vote(peer, message)).Times(1);

  network->subscribe(notifications);

  std::unique_ptr<grpc::Server> server;

  grpc::ServerBuilder builder;
  int port = 0;
  builder.AddListeningPort(peer.address, grpc::InsecureServerCredentials(),
                           &port);
  builder.RegisterService(network.get());
  server = builder.BuildAndStart();
  ASSERT_TRUE(server);
  ASSERT_NE(port, 0);

  network->send_vote(peer, message);

  // wait for response reader thread
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait_for(lock, std::chrono::milliseconds(100));

  server->Shutdown();
}
