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
#include "logger/logger.hpp"

using namespace iroha::consensus::yac;

static logger::Logger log_ = logger::testLog("YacStorage");

TEST(YacStorageTest, SupermajorityFunctionForAllCases2) {
  log_->info("-----------| F(x, 2), x in {0..3} -----------");

  int N = 2;
  ASSERT_FALSE(hasSupermajority(0, N));
  ASSERT_FALSE(hasSupermajority(1, N));
  ASSERT_TRUE(hasSupermajority(2, N));
  ASSERT_FALSE(hasSupermajority(3, N));
}

TEST(YacStorageTest, SupermajorityFunctionForAllCases4) {
  log_->info("-----------| F(x, 4), x in {0..5} |-----------");

  int N = 4;
  ASSERT_FALSE(hasSupermajority(0, N));
  ASSERT_FALSE(hasSupermajority(1, N));
  ASSERT_FALSE(hasSupermajority(2, N));
  ASSERT_TRUE(hasSupermajority(3, N));
  ASSERT_TRUE(hasSupermajority(4, N));
  ASSERT_FALSE(hasSupermajority(5, N));
}

TEST(YacStorageTest, YacBlockStorageWhenNormalDataInput) {
  log_->info("-----------| Sequentially insertion of votes |-----------");

  YacHash hash("proposal", "commit");
  int N = 4;
  YacBlockStorage storage(hash, N);

  auto insert_1 = storage.insert(create_vote(hash, "one"));
  ASSERT_NE(nonstd::nullopt, insert_1);
  ASSERT_EQ(nonstd::nullopt, insert_1->hash);
  ASSERT_EQ(nonstd::nullopt, insert_1->commit);
  ASSERT_EQ(nonstd::nullopt, insert_1->reject);

  auto insert_2 = storage.insert(create_vote(hash, "two"));
  ASSERT_NE(nonstd::nullopt, insert_2);
  ASSERT_EQ(nonstd::nullopt, insert_2->hash);
  ASSERT_EQ(nonstd::nullopt, insert_2->commit);
  ASSERT_EQ(nonstd::nullopt, insert_2->reject);

  auto insert_3 = storage.insert(create_vote(hash, "three"));
  ASSERT_NE(nonstd::nullopt, insert_3);
  ASSERT_EQ(hash.proposal_hash, insert_3->hash.value());
  ASSERT_NE(nonstd::nullopt, insert_3->commit);
  ASSERT_EQ(3, insert_3->commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_3->reject);

  auto insert_4 = storage.insert(create_vote(hash, "four"));
  ASSERT_NE(nonstd::nullopt, insert_4);
  ASSERT_EQ(hash.proposal_hash, insert_4->hash.value());
  ASSERT_NE(nonstd::nullopt, insert_4->commit);
  ASSERT_EQ(4, insert_4->commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_4->reject);
}

TEST(YacStorageTest, YacBlockStorageWhenNotCommittedAndCommitAcheive) {
  log_->info("-----------| Insert vote => insert commit |-----------");

  YacHash hash("proposal", "commit");
  int N = 4;
  YacBlockStorage storage(hash, N);

  auto insert_1 = storage.insert(create_vote(hash, "one"));
  ASSERT_NE(nonstd::nullopt, insert_1);
  ASSERT_EQ(nonstd::nullopt, insert_1->hash);
  ASSERT_EQ(nonstd::nullopt, insert_1->commit);
  ASSERT_EQ(nonstd::nullopt, insert_1->reject);

  auto insert_commit = storage.insert({create_vote(hash, "two"),
                                       create_vote(hash, "three"),
                                       create_vote(hash, "four")}
  );
  ASSERT_EQ(hash.proposal_hash, insert_commit.hash.value());
  ASSERT_EQ(4, insert_commit.commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_1->reject);
}
