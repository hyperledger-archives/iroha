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

#include <gmock/gmock.h>
#include <grpc++/grpc++.h>
#include <gtest/gtest.h>
#include <thread>
#include "consensus/yac/impl/network_impl.hpp"

using namespace iroha::consensus::yac;
using iroha::model::Peer;

using ::testing::_;

/**
 * Mock for network notifications
 */
class FakeNetworkNotifications : public YacNetworkNotifications {
 public:
  MOCK_METHOD2(on_commit, void(Peer, CommitMessage));
  MOCK_METHOD2(on_reject, void(Peer, RejectMessage));
  MOCK_METHOD2(on_vote, void(Peer, VoteMessage));
};

TEST(NetworkTest, MessageHandledWhenMessageSent) {
  auto notifications = std::make_shared<FakeNetworkNotifications>();
  EXPECT_CALL(*notifications, on_vote(_, _)).Times(1);

  auto peer = Peer();
  peer.address = "0.0.0.0:50051";
  std::vector<Peer> peers = {peer};
  std::shared_ptr<YacNetwork> network =
      std::make_shared<NetworkImpl>(peer.address, peers);

  network->subscribe(notifications);

  network->send_vote(peer, VoteMessage());
}