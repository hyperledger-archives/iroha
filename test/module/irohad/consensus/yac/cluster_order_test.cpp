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

#include "consensus/yac/cluster_order.hpp"
#include <gtest/gtest.h>

TEST(ClusterOrderTest, ClusterOrderOnNext) {
  iroha::model::Peer p1;
  p1.address = "1";
  iroha::model::Peer p2;
  p2.address = "2";
  std::vector<iroha::model::Peer> peers = {p1, p2};
  iroha::consensus::yac::ClusterOrdering order(peers);
  ASSERT_EQ("1", order.currentLeader().address);
  ASSERT_EQ("2", order.switchToNext().currentLeader().address);
  ASSERT_EQ("1", order.switchToNext().currentLeader().address);
}

TEST(ClusterOrderTest, ClusterOrderLeaderSet) {
  iroha::model::Peer p;
  std::vector<iroha::model::Peer> peers = {p, p, p, p, p, p, p};
  iroha::consensus::yac::ClusterOrdering order(peers);
  ASSERT_EQ(order.leaderInValidateSet(), true);  // 1
  order.switchToNext();
  ASSERT_EQ(order.leaderInValidateSet(), true);  // 2
  order.switchToNext();
  ASSERT_EQ(order.leaderInValidateSet(), true);  // 3
  order.switchToNext();
  ASSERT_EQ(order.leaderInValidateSet(), true);  // 4
  order.switchToNext();
  ASSERT_EQ(order.leaderInValidateSet(), true);  // 5
  order.switchToNext();
  ASSERT_EQ(order.leaderInValidateSet(), false);  // 6
  order.switchToNext();
  ASSERT_EQ(order.leaderInValidateSet(), false);  // 7
  order.switchToNext();
}

TEST(ClusterOrderTest, ClusterOrderSupermajority) {
  iroha::model::Peer p;
  std::vector<iroha::model::Peer> peers = {p, p, p, p, p, p, p};
  iroha::consensus::yac::ClusterOrdering order(peers);
  ASSERT_EQ(order.haveSupermajority(1), false);  // 1
  ASSERT_EQ(order.haveSupermajority(2), false);  // 2
  ASSERT_EQ(order.haveSupermajority(3), false);  // 3
  ASSERT_EQ(order.haveSupermajority(4), false);  // 4
  ASSERT_EQ(order.haveSupermajority(5), true);   // 5
  ASSERT_EQ(order.haveSupermajority(6), true);   // 6
  ASSERT_EQ(order.haveSupermajority(7), true);   // 7
}

TEST(ClusterOrderTest, CluserOrderSupermajorityWhenEmpty) {
  iroha::consensus::yac::ClusterOrdering order;
  ASSERT_EQ(order.haveSupermajority(1), false);  // 1
  ASSERT_EQ(order.haveSupermajority(2), false);  // 2
  ASSERT_EQ(order.haveSupermajority(3), false);  // 3
  ASSERT_EQ(order.haveSupermajority(4), false);  // 4
  ASSERT_EQ(order.haveSupermajority(5), false);  // 5
  ASSERT_EQ(order.haveSupermajority(6), false);  // 6
  ASSERT_EQ(order.haveSupermajority(7), false);  // 7
}