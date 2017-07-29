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

#ifndef IROHA_YAC_MOCKS_HPP
#define IROHA_YAC_MOCKS_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "consensus/yac/yac.hpp"
#include "consensus/yac/yac_gate.hpp"
#include "consensus/yac/cluster_order.hpp"
#include "consensus/yac/yac_hash_provider.hpp"
#include "consensus/yac/yac_peer_orderer.hpp"
#include "consensus/yac/messages.hpp"

using namespace iroha::consensus::yac;
using iroha::model::Peer;

Peer mk_peer(std::string address) {
  Peer peer;
  peer.address = address;
  return peer;
}

VoteMessage create_vote(YacHash hash, std::string sign) {
  VoteMessage vote;
  vote.hash = hash;
  std::copy(sign.begin(), sign.end(),
            vote.signature.pubkey.begin());
  return vote;
}

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

  CryptoProviderMock() {};

  CryptoProviderMock(const CryptoProviderMock &) {};

  CryptoProviderMock &operator=(const CryptoProviderMock &) { return *this; };
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

  FakeTimer() {};

  FakeTimer(const FakeTimer &rhs) {};

  FakeTimer &operator=(const FakeTimer &rhs) { return *this; };
};

/**
 * Mock for network
 */
class FakeNetwork : public YacNetwork {
 public:
  void subscribe(std::shared_ptr<YacNetworkNotifications> handler) override {
    notification = handler;
  };

  void release() { notification.reset(); }

  MOCK_METHOD2(send_commit, void(Peer, CommitMessage));
  MOCK_METHOD2(send_reject, void(Peer, RejectMessage));
  MOCK_METHOD2(send_vote, void(Peer, VoteMessage));

  FakeNetwork() {};

  FakeNetwork(const FakeNetwork &rhs) { notification = rhs.notification; };

  FakeNetwork &operator=(const FakeNetwork &rhs) {
    notification = rhs.notification;
    return *this;
  };

  FakeNetwork(FakeNetwork &&rhs) { std::swap(notification, rhs.notification); };

  FakeNetwork &operator=(FakeNetwork &&rhs) {
    std::swap(notification, rhs.notification);
    return *this;
  };

  std::shared_ptr<YacNetworkNotifications> notification;
};

class HashGateMock : public HashGate {
 public:
  MOCK_METHOD2(vote, void(YacHash, ClusterOrdering));
  MOCK_METHOD0(on_commit, rxcpp::observable<CommitMessage>());

  HashGateMock() = default;

  HashGateMock(const HashGateMock &rhs) {

  };

  HashGateMock(HashGateMock &&rhs) {
  };

  HashGateMock &operator=(const HashGateMock &rhs) {
    return *this;
  };
};

class YacPeerOrdererMock : public YacPeerOrderer {
 public:
  MOCK_METHOD0(getInitialOrdering, nonstd::optional<ClusterOrdering>());
  MOCK_METHOD1(getOrdering, nonstd::optional<ClusterOrdering>(YacHash));

  YacPeerOrdererMock() = default;

  YacPeerOrdererMock(const YacPeerOrdererMock &rhs) {
  };

  YacPeerOrdererMock(YacPeerOrdererMock &&rhs) {
  };

  YacPeerOrdererMock &operator=(const YacPeerOrdererMock &rhs) {
    return *this;
  };
};

class YacHashProviderMock : public YacHashProvider {
 public:
  MOCK_METHOD1(makeHash, YacHash(iroha::model::Block));

  YacHashProviderMock() = default;

  YacHashProviderMock(const YacHashProviderMock &rhs) {
  };

  YacHashProviderMock(YacHashProviderMock &&rhs) {
  };

  YacHashProviderMock &operator=(const YacHashProviderMock &rhs) {
    return *this;
  };
};

class YacTest : public ::testing::Test {
 public:
  // ------|Netowrk|------
  std::shared_ptr<FakeNetwork> network;
  std::shared_ptr<CryptoProviderMock> crypto;
  std::shared_ptr<FakeTimer> timer;
  uint64_t delay = 100500;
  std::shared_ptr<Yac> yac;

  // ------|Round|------
  std::vector<Peer> default_peers = {mk_peer("1"), mk_peer("2"), mk_peer("3"),
                                     mk_peer("4"), mk_peer("5"), mk_peer("6"),
                                     mk_peer("7")};

  virtual void SetUp() override {
    network = std::make_shared<FakeNetwork>();
    crypto = std::make_shared<CryptoProviderMock>();
    timer = std::make_shared<FakeTimer>();
    yac = Yac::create(std::move(YacVoteStorage()), network, crypto,
                      timer, ClusterOrdering(default_peers), delay);
    network->subscribe(yac);
  };

  virtual void TearDown() override { network->release(); };
};

#endif  // IROHA_YAC_MOCKS_HPP
