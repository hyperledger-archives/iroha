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

#include "ametsuchi/impl/redis_flat_block_query.hpp"
#include "ametsuchi/impl/storage_impl.hpp"
#include "crypto/hash.hpp"
#include "framework/test_subscriber.hpp"
#include "module/irohad/ametsuchi/ametsuchi_fixture.hpp"

using namespace iroha::ametsuchi;
using namespace iroha::model;
using namespace framework::test_subscriber;

/**
 * @given block store with 2 blocks totally containing 3 txs created by
 * user1@test and 1 tx created by user2@test
 * @when queries to get transactions created by both users are invoked
 * @then query over user1@test returns 3 txs and query over user2@test returns 1
 * tx and query over non-existing user returns 0 txs
 */
TEST_F(AmetsuchiTest, GetAccountTransactions) {
  auto storage =
      StorageImpl::create(block_store_path, redishost_, redisport_, pgopt_);

  ASSERT_TRUE(storage);

  auto blocks = storage->getBlockQuery();

  std::string creator1 = "user1@test";
  std::string creator2 = "user2@test";

  // First transaction in block1
  Transaction txn1_1;
  txn1_1.creator_account_id = creator1;

  // Second transaction in block1
  Transaction txn1_2;
  txn1_2.creator_account_id = creator1;

  Block block1;
  block1.height = 1;
  block1.transactions.push_back(txn1_1);
  block1.transactions.push_back(txn1_2);
  auto block1hash = iroha::hash(block1);

  // First tx in block 1
  Transaction txn2_1;
  txn2_1.creator_account_id = creator1;

  // Second tx in block 2
  Transaction txn2_2;
  // this tx has another creator
  txn2_2.creator_account_id = creator2;

  Block block2;
  block2.height = 2;
  block2.prev_hash = block1hash;
  block2.transactions.push_back(txn2_1);
  block2.transactions.push_back(txn2_2);

  auto ms = storage->createMutableStorage();
  ms->apply(block1, [](const auto &, auto &, const auto &) { return true; });
  ms->apply(block2, [](const auto &, auto &, const auto &) { return true; });
  storage->commit(std::move(ms));

  // Check that creator1 has created 3 transactions
  auto getCreator1TxWrapper = make_test_subscriber<CallExact>(
      blocks->getAccountTransactions(creator1), 3);
  getCreator1TxWrapper.subscribe(
      [&creator1](auto val) { EXPECT_EQ(val.creator_account_id, creator1); });
  ASSERT_TRUE(getCreator1TxWrapper.validate());

  // Check that creator1 has created 1 transaction
  auto getCreator2TxWrapper = make_test_subscriber<CallExact>(
      blocks->getAccountTransactions(creator2), 1);
  getCreator2TxWrapper.subscribe(
      [&creator2](auto val) { EXPECT_EQ(val.creator_account_id, creator2); });
  ASSERT_TRUE(getCreator2TxWrapper.validate());

  // Check that "nonexisting" user has no transaction
  auto getNonexistingTxWrapper = make_test_subscriber<CallExact>(
      blocks->getAccountTransactions("nonexisting user"), 0);
  getNonexistingTxWrapper.subscribe(
      [&creator2](auto val) { EXPECT_TRUE(false); });
  ASSERT_TRUE(getNonexistingTxWrapper.validate());
}
