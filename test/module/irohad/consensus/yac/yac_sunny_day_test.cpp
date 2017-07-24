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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "yac_mocks.hpp"
#include "common/test_observable.hpp"

#include <vector>
#include <iostream>

using ::testing::Return;
using ::testing::_;
using ::testing::An;
using ::testing::AtLeast;

using namespace iroha::consensus::yac;
using namespace common::test_observable;
using namespace std;

TEST_F(YacTest, ValidCase) {
  cout << "-----------|Start => vote => commit|-----------" << endl;

  auto my_peers = std::vector<iroha::model::Peer>(
      {default_peers.begin(), default_peers.begin() + 4});
  ASSERT_EQ(4, my_peers.size());

  ClusterOrdering my_order(my_peers);

  // delay preference
  uint64_t wait_seconds = 10;
  delay = wait_seconds * 1000;

  yac = Yac::create(network, crypto, timer, my_order, delay);

  EXPECT_CALL(*network, send_commit(_, _)).Times(1);
  EXPECT_CALL(*network, send_reject(_, _)).Times(0);
  EXPECT_CALL(*network, send_vote(_, _)).Times(my_peers.size());

  EXPECT_CALL(*crypto, verify(An<CommitMessage>())).Times(0);
  EXPECT_CALL(*crypto, verify(An<RejectMessage>())).Times(0);
  EXPECT_CALL(*crypto, verify(An<VoteMessage>())).WillRepeatedly(Return(true));

  // wait sometime
  // todo wait here

  YacHash my_hash("proposal_hash", "block_hash");
  yac->vote(my_hash, my_order);

  for (auto i = 0; i < 3; ++i) {
    yac->on_vote(my_peers.at(i), crypto->getVote(my_hash));
  };

}