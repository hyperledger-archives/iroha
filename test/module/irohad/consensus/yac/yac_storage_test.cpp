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
#include <algorithm>
#include "consensus/yac/storage/yac_vote_storage.hpp"
#include "yac_mocks.hpp"

using namespace iroha::consensus::yac;

TEST(YacStorageTest, SupermajorityFunctionForAllCases2) {
  int N = 2;
  ASSERT_EQ(false, hasSupermajority(0, N));
  ASSERT_EQ(false, hasSupermajority(1, N));
  ASSERT_EQ(true, hasSupermajority(2, N));
  ASSERT_EQ(false, hasSupermajority(3, N));
}

TEST(YacStorageTest, SupermajorityFunctionForAllCases4) {
  int N = 4;
  ASSERT_EQ(false, hasSupermajority(0, N));
  ASSERT_EQ(false, hasSupermajority(1, N));
  ASSERT_EQ(false, hasSupermajority(2, N));
  ASSERT_EQ(true, hasSupermajority(3, N));
  ASSERT_EQ(true, hasSupermajority(4, N));
  ASSERT_EQ(false, hasSupermajority(5, N));
}

TEST(YacStorageTest, YacBlockStorageWhenNormalDataInput) {
  YacHash hash("proposal", "commit");
  int N = 4;
  YacBlockStorage storage(hash, N);

  auto insert_1 = storage.insert(create_vote(hash, "one"));
  ASSERT_EQ(CommitState::not_committed, insert_1.state);
  ASSERT_EQ(nonstd::nullopt, insert_1.answer.commit);
  ASSERT_EQ(nonstd::nullopt, insert_1.answer.reject);

  auto insert_2 = storage.insert(create_vote(hash, "two"));
  ASSERT_EQ(CommitState::not_committed, insert_2.state);
  ASSERT_EQ(nonstd::nullopt, insert_2.answer.commit);
  ASSERT_EQ(nonstd::nullopt, insert_2.answer.reject);

  auto insert_3 = storage.insert(create_vote(hash, "three"));
  ASSERT_EQ(CommitState::committed, insert_3.state);
  ASSERT_NE(nonstd::nullopt, insert_3.answer.commit);
  ASSERT_EQ(3, insert_3.answer.commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_3.answer.reject);

  auto insert_4 = storage.insert(create_vote(hash, "four"));
  ASSERT_EQ(CommitState::committed_before, insert_4.state);
  ASSERT_EQ(4, insert_4.answer.commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_4.answer.reject);
}

TEST(YacStorageTest, YacBlockStorageWhenNotCommittedAndCommitAcheive) {
  YacHash hash("proposal", "commit");
  int N = 4;
  YacBlockStorage storage(hash, N);

  auto insert_1 = storage.insert(create_vote(hash, "one"));
  ASSERT_EQ(CommitState::not_committed, insert_1.state);
  ASSERT_EQ(nonstd::nullopt, insert_1.answer.commit);
  ASSERT_EQ(nonstd::nullopt, insert_1.answer.reject);

  auto insert_commit = storage.insert(CommitMessage({create_vote(hash, "two"),
                                                     create_vote(hash, "three"),
                                                     create_vote(hash, "four")})
  );
  ASSERT_EQ(CommitState::committed, insert_commit.state);
  ASSERT_EQ(4, insert_commit.answer.commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_1.answer.reject);
}
