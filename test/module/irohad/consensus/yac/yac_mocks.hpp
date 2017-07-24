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
#include "consensus/yac/yac.hpp"

using namespace iroha::consensus::yac;
using iroha::model::Peer;

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

#endif //IROHA_YAC_MOCKS_HPP
