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

#include <boost/optional.hpp>
#include "ametsuchi/impl/redis_block_index.hpp"
#include "ametsuchi/impl/redis_block_query.hpp"
#include "crypto/hash.hpp"
#include "framework/test_subscriber.hpp"
#include "model/commands/transfer_asset.hpp"
#include "module/irohad/ametsuchi/ametsuchi_fixture.hpp"

using namespace framework::test_subscriber;

namespace iroha {
  namespace ametsuchi {
    class BlockQueryTransferTest : public AmetsuchiTest {
     protected:
      void SetUp() override {
        AmetsuchiTest::SetUp();

        file = FlatFile::create(block_store_path);
        ASSERT_TRUE(file);
        index = std::make_shared<RedisBlockIndex>(client);
        blocks = std::make_shared<RedisBlockQuery>(client, *file);

        model::Transaction txn;
        txn.creator_account_id = creator1;

        auto cmd = std::make_shared<model::TransferAsset>();
        cmd->src_account_id = creator1;
        cmd->dest_account_id = creator2;
        cmd->asset_id = asset;
        txn.commands.push_back(cmd);
        tx_hashes.push_back(iroha::hash(txn));

        model::Block block;
        block.height = 1;
        block.transactions.push_back(txn);

        file->add(block.height,
                  iroha::stringToBytes(model::converters::jsonToString(
                      model::converters::JsonBlockFactory().serialize(block))));
        index->index(block);
      }

      std::vector<iroha::hash256_t> tx_hashes;
      std::shared_ptr<BlockQuery> blocks;
      std::shared_ptr<BlockIndex> index;
      std::unique_ptr<FlatFile> file;
      std::string creator1 = "user1@test";
      std::string creator2 = "user2@test";
      std::string asset = "coin#test";
    };

    /**
     * @given block store and index with block containing 1 transaction
     * from creator 1 with transfer to creator 2 sending asset
     * @when query to get asset transactions of sender
     * @then query returns the transaction
     */
    TEST_F(BlockQueryTransferTest, SenderAssetName) {
      auto wrapper = make_test_subscriber<CallExact>(
          blocks->getAccountAssetTransactions(creator1, asset), 1);
      wrapper.subscribe(
          [this](auto val) { ASSERT_EQ(tx_hashes.at(0), iroha::hash(val)); });
      ASSERT_TRUE(wrapper.validate());
    }

    /**
     * @given block store and index with block containing 1 transaction
     * from creator 1 with transfer to creator 2 sending asset
     * @when query to get asset transactions of receiver
     * @then query returns the transaction
     */
    TEST_F(BlockQueryTransferTest, ReceiverAssetName) {
      auto wrapper = make_test_subscriber<CallExact>(
          blocks->getAccountAssetTransactions(creator2, asset), 1);
      wrapper.subscribe(
          [this](auto val) { ASSERT_EQ(tx_hashes.at(0), iroha::hash(val)); });
      ASSERT_TRUE(wrapper.validate());
    }
  }  // namespace ametsuchi
}  // namespace iroha
