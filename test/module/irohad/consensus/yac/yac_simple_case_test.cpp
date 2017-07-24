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

#ifndef IROHA_YAC_SIMPLE_CASE_TEST_HPP
#define IROHA_YAC_SIMPLE_CASE_TEST_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <utility>
#include <memory>
#include <vector>
#include <iostream>
#include "consensus/yac/yac.hpp"
#include "common/test_observable.hpp"

using ::testing::Return;
using ::testing::_;
using ::testing::An;

using namespace iroha::consensus::yac;
using namespace common::test_observable;
using namespace std;

/**
 * Mock for yac crypto provider
 */
class CryptoProviderMock : public YacCryptoProvider {
 public:
  MOCK_METHOD1(verify, bool(CommitMessage));
  MOCK_METHOD1(verify, bool(RejectMessage));
  MOCK_METHOD1(verify, bool(VoteMessage));

  VoteMessage getVote(YacHash hash) override {
    VoteMessage vote;
    vote.hash = hash;
    return vote;
  };

  CryptoProviderMock() {
  };

  CryptoProviderMock(const CryptoProviderMock &) {
  };

  CryptoProviderMock &operator=(const CryptoProviderMock &) const {
  };
};

/**
 * Mock for timer
 */
class FakeTimer : public Timer {
 public:
  void invokeAfterDelay(uint64_t millis,
                        std::function<void()> handler) override {
    handler();
  };

  MOCK_METHOD0(deny, void());

  FakeTimer() {
  };

  FakeTimer(const FakeTimer &rhs) {
  };

  FakeTimer &operator=(const FakeTimer &rhs) {
    return *this;
  };
};

using iroha::model::Peer;

/**
 * Mock for network
 */
class FakeNetwork : public YacNetwork {
 public:

  void subscribe(std::shared_ptr<YacNetworkNotifications> handler) override {
    notification = handler;
  };

  void release() {
    notification.reset();
  }

  MOCK_METHOD2(send_commit, void(Peer, CommitMessage));
  MOCK_METHOD2(send_reject, void(Peer, RejectMessage));
  MOCK_METHOD2(send_vote, void(Peer, VoteMessage));

  FakeNetwork() {
  };

  FakeNetwork(const FakeNetwork &rhs) {
    notification = rhs.notification;
  };

  FakeNetwork &operator=(const FakeNetwork &rhs) {
    notification = rhs.notification;
    return *this;
  };

  FakeNetwork(FakeNetwork &&rhs) {
    std::swap(notification, rhs.notification);
  };

  FakeNetwork &operator=(FakeNetwork &&rhs) {
    std::swap(notification, rhs.notification);
    return *this;
  };

  std::shared_ptr<YacNetworkNotifications> notification;
};

Peer f_peer(std::string address) {
  Peer peer;
  peer.address = address;
  return peer;
}

class YacTest : public ::testing::Test {
 public:
  // ------|Netowrk|------
  std::shared_ptr<FakeNetwork> network;
  std::shared_ptr<CryptoProviderMock> crypto;
  std::shared_ptr<FakeTimer> timer;
  uint64_t delay = 100500;
  std::shared_ptr<Yac> yac;

  // ------|Round|------
  std::vector<Peer> default_peers = {f_peer("1"),
                                     f_peer("2"),
                                     f_peer("3"),
                                     f_peer("4"),
                                     f_peer("5"),
                                     f_peer("6"),
                                     f_peer("7")
  };

  virtual void SetUp() override {
    network = std::make_shared<FakeNetwork>();
    crypto = std::make_shared<CryptoProviderMock>();
    timer = std::make_shared<FakeTimer>();
    yac = Yac::create(network,
                      crypto,
                      timer,
                      delay);
    network->subscribe(yac);
  }

  virtual void TearDown() override {
    network->release();
  }
};

/**
 * Test provide use case for init yac object
 */
TEST_F(YacTest, YacWhenInit) {
  cout << "----------|YacWhenInit|----------" << endl;

  FakeNetwork network_;

  CryptoProviderMock crypto_;

  FakeTimer timer_;

  auto fake_delay_ = 100500;

  auto yac_ = Yac::create(std::make_shared<FakeNetwork>(network_),
                          std::make_shared<CryptoProviderMock>(crypto_),
                          std::make_shared<FakeTimer>(timer_),
                          fake_delay_);

  network_.subscribe(yac_);
}

/**
 * Test provide scenario when yac vote for hash
 */
TEST_F(YacTest, YacWhenVoting) {
  cout << "----------|YacWhenAchieveOneVote|----------" << endl;

  EXPECT_CALL(*network, send_commit(_, _)).Times(0);
  EXPECT_CALL(*network, send_reject(_, _)).Times(0);
  EXPECT_CALL(*network, send_vote(_, _)).Times(default_peers.size());

  YacHash my_hash("my_proposal_hash", "my_block_hash");
  yac->vote(my_hash, default_peers);
}

/**
 * Test provide scenario when yac cold started and achieve one vote
 */
TEST_F(YacTest, YacWhenColdStartAndAchieveOneVote) {
  cout << "----------|Coldstart - one vote|----------" << endl;

  EXPECT_CALL(*network, send_commit(_, _)).Times(0);
  EXPECT_CALL(*network, send_reject(_, _)).Times(0);
  EXPECT_CALL(*network, send_vote(_, _)).Times(0);

  EXPECT_CALL(*crypto, verify(An<CommitMessage>()))
      .Times(0);
  EXPECT_CALL(*crypto, verify(An<RejectMessage>()))
      .Times(0);
  EXPECT_CALL(*crypto, verify(An<VoteMessage>()))
      .Times(1)
      .WillRepeatedly(Return(true));

  YacHash received_hash("my_proposal", "my_block");
  auto peer = default_peers.at(0);
  // assume that our peer receive message
  network->notification->on_vote(peer, crypto->getVote(received_hash));
}

/**
 * Test provide scenario
 * when yac cold started and achieve supermajority of  votes
 */
TEST_F(YacTest, YacWhenColdStartAndAchieveSupermajorityOfVotes) {
  cout << "----------|Coldstart - supermajority of votes|----------" << endl;

  EXPECT_CALL(*network, send_commit(_, _)).Times(0);
  EXPECT_CALL(*network, send_reject(_, _)).Times(0);
  EXPECT_CALL(*network, send_vote(_, _)).Times(0);

  EXPECT_CALL(*crypto, verify(An<CommitMessage>()))
      .Times(0);
  EXPECT_CALL(*crypto, verify(An<RejectMessage>()))
      .Times(0);
  EXPECT_CALL(*crypto, verify(An<VoteMessage>()))
      .Times(default_peers.size())
      .WillRepeatedly(Return(true));

  YacHash received_hash("my_proposal", "my_block");
  for (auto &peer : default_peers) {
    network->notification->on_vote(peer, crypto->getVote(received_hash));
  }
}

/**
 * Test provide scenario
 * when yac cold started and achieve commit
 */
TEST_F(YacTest, YacWhenColdStartAndAchieveCommitMessage) {
  cout << "----------|Coldstart - commit received "
      "(commit inside case)"
      "|----------"
       << endl;
  YacHash propagated_hash("my_proposal", "my_block");

  // verify that commit emitted
  TestObservable<YacHash> wrapper(yac->on_commit());
  auto invariant = CallExact<YacHash>(1);
  wrapper.test_subscriber(std::make_unique<CallExact<YacHash>>(
      std::move(invariant)), [propagated_hash](auto commit_hash) {
    ASSERT_EQ(propagated_hash, commit_hash);
  });

  EXPECT_CALL(*network, send_commit(_, _)).Times(0);
  EXPECT_CALL(*network, send_reject(_, _)).Times(0);
  EXPECT_CALL(*network, send_vote(_, _)).Times(0);

  EXPECT_CALL(*crypto, verify(An<CommitMessage>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*crypto, verify(An<RejectMessage>()))
      .Times(0);
  EXPECT_CALL(*crypto, verify(An<VoteMessage>()))
      .Times(0);

  auto committed_peer = default_peers.at(0);
  auto msg = CommitMessage();
  for (auto &peer : default_peers) {
    msg.votes.push_back(crypto->getVote(propagated_hash));
  }
  network->notification->on_commit(committed_peer, msg);
  ASSERT_EQ(true, invariant.validate());
}
#endif //IROHA_YAC_SIMPLE_CASE_TEST_HPP
