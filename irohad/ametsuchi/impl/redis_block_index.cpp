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

#include "ametsuchi/impl/redis_block_index.hpp"

#include <boost/format.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/numeric.hpp>
#include <unordered_map>
#include <unordered_set>

#include "crypto/hash.hpp"
#include "model/commands/transfer_asset.hpp"

namespace iroha {
  namespace ametsuchi {

    RedisBlockIndex::RedisBlockIndex(cpp_redis::redis_client &client)
        : client_(client) {}

    void RedisBlockIndex::index(const model::Block &block) {
      const auto &height = std::to_string(block.height);

      boost::format account_id_height("%s:%s"),
          account_id_height_asset_id("%s:%s:%s");

      boost::for_each(
          block.transactions | boost::adaptors::indexed(0),
          [&](const auto &tx) {
            const auto &creator_id = tx.value().creator_account_id;
            const auto &hash = iroha::hash(tx.value()).to_string();
            const auto &index = std::to_string(tx.index());

            // tx hash -> block where hash is stored
            client_.set(hash, height);

            // to make index account_id -> list of blocks where his txs exist
            client_.sadd(creator_id, {height});

            // to make index account_id:height -> list of tx indexes (where
            // tx is placed in the block)
            client_.rpush(boost::str(account_id_height % creator_id % height),
                          {index});

            // collect all assets belonging to creator, sender, and receiver
            // to make account_id:height:asset_id -> list of tx indexes (where
            // tx with certain asset is placed in the block )
            boost::accumulate(
                tx.value().commands
                    // filter transfers
                    | boost::adaptors::filtered([](const auto &cmd) {
                        return instanceof <model::TransferAsset>(*cmd);
                      })
                    // map pointer to an abstract command to
                    // reference to a transfer command
                    | boost::adaptors::transformed(
                          [](const auto &cmd) -> decltype(auto) {
                            return (
                                *std::static_pointer_cast<model::TransferAsset>(
                                    cmd));
                          }),
                std::unordered_map<std::string,
                                   std::unordered_set<std::string>>(),
                [&](auto &&acc, const auto &cmd) {
                  for (const auto &id :
                       {cmd.src_account_id, cmd.dest_account_id}) {
                    client_.sadd(id, {height});
                  }

                  auto ids = {
                      creator_id, cmd.src_account_id, cmd.dest_account_id};
                  boost::for_each(
                      ids
                          // map id to account_id:height:asset_id
                          | boost::adaptors::transformed([&](const auto &id) {
                              return boost::str(account_id_height_asset_id % id
                                                % block.height
                                                % cmd.asset_id);
                            })
                          // filter unindexed values
                          | boost::adaptors::filtered([&](const auto &key) {
                              return acc[key].insert(index).second;
                            }),
                      [&](const auto &key) { client_.rpush(key, {index}); });

                  return std::forward<decltype(acc)>(acc);
                });
          });
    }
  }  // namespace ametsuchi
}  // namespace iroha
