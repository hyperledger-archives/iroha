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

#include "genesis_block_processor_impl.hpp"

namespace iroha {
  bool GenesisBlockProcessorImpl::genesis_block_handle(const iroha::model::Block &block) {

    model::Block block;
    block.transactions.push_back(txn);
    block.height = 1;
    block.prev_hash.fill(0);
    auto block1hash = hashProvider.get_hash(block);
    block.hash = block1hash;
    block.txs_number = block.transactions.size();

    {
      auto ms = storage->createMutableStorage();
      ms->apply(block, [](const auto &blk, auto &executor, auto &query,
                          const auto &top_hash) {
        EXPECT_TRUE(
          blk.transactions.at(0).commands.at(0)->execute(query, executor));
        EXPECT_TRUE(
          blk.transactions.at(0).commands.at(1)->execute(query, executor));
        return true;
      });
      storage->commit(std::move(ms));
    }

    auto mutable_storage = mutable_factory_.createMutableStorage();
    mutable_storage->apply(block, [](const auto &block, auto &executor, auto &query,
                                     const auto &top_hash) {
      for (const auto &tx : block.transactions) {
        for (const auto &command : tx.commands) {
          auto valid = command->execute(query, executor);
          if (!valid) return false;
        }
      }
    });
    mutable_factory_.commit(std::move(mutable_storage));
  }
}
