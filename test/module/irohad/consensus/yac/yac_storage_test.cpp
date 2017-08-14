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
#include "consensus/yac/storage/yac_vote_storage.hpp"
#include "module/irohad/consensus/yac/yac_mocks.hpp"
#include "logger/logger.hpp"

using namespace iroha::consensus::yac;

static logger::Logger log_ = logger::testLog("YacStorage");

TEST(YacStorageTest, YacBlockStorageWhenNormalDataInput) {
  log_->info("-----------| Sequentially insertion of votes |-----------");

  YacHash hash("proposal", "commit");
  int N = 4;
  YacBlockStorage storage(hash, N);

  auto insert_1 = storage.insert(create_vote(hash, "one"));
  ASSERT_EQ(nonstd::nullopt, insert_1);

  auto insert_2 = storage.insert(create_vote(hash, "two"));
  ASSERT_EQ(nonstd::nullopt, insert_2);

  auto insert_3 = storage.insert(create_vote(hash, "three"));
  ASSERT_NE(nonstd::nullopt, insert_3);
  ASSERT_NE(nonstd::nullopt, insert_3->commit);
  ASSERT_EQ(3, insert_3->commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_3->reject);

  auto insert_4 = storage.insert(create_vote(hash, "four"));
  ASSERT_NE(nonstd::nullopt, insert_4);
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
  ASSERT_EQ(nonstd::nullopt, insert_1);

  auto insert_commit = storage.insert({create_vote(hash, "two"),
                                       create_vote(hash, "three"),
                                       create_vote(hash, "four")}
  );
  ASSERT_EQ(4, insert_commit->commit->votes.size());
  ASSERT_EQ(nonstd::nullopt, insert_commit->reject);
}
