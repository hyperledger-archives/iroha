/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
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

#include "ametsuchi/impl/postgres_block_index.hpp"

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/numeric.hpp>
#include <unordered_map>
#include <unordered_set>

#include "cryptography/ed25519_sha3_impl/internal/sha3_hash.hpp"
#include "model/commands/transfer_asset.hpp"
#include "model/sha3_hash.hpp"

namespace iroha {
  namespace ametsuchi {

    PostgresBlockIndex::PostgresBlockIndex(pqxx::nontransaction &transaction)
        : transaction_(transaction),
          log_(logger::log("PostgresBlockIndex")),
          execute_{makeExecute(transaction_, log_)} {}

    void PostgresBlockIndex::index(const model::Block &block) {
      const auto &height = std::to_string(block.height);
      boost::for_each(block.transactions | boost::adaptors::indexed(0),
                      [&](const auto &tx) {
                        const auto &creator_id = tx.value().creator_account_id;
                        const auto &hash = iroha::hash(tx.value()).to_string();
                        const auto &index = std::to_string(tx.index());

                        // tx hash -> block where hash is stored
                        execute_("INSERT INTO height_by_hash(hash, height) VALUES ("
                                 + transaction_.quote(
                                pqxx::binarystring(hash.data(), hash.size()))
                                 + ", " + transaction_.quote(height) + ");");

                        this->indexAccountIdHeight(creator_id, height);

                        // to make index account_id:height -> list of tx indexes
                        // (where tx is placed in the block)
                        execute_(
                            "INSERT INTO index_by_creator_height(creator_id, "
                            "height, index) "
                            "VALUES ("
                            + transaction_.quote(creator_id) + ", "
                            + transaction_.quote(height) + ", "
                            + transaction_.quote(index) + ");");

                        this->indexAccountAssets(
                            creator_id, height, index, tx.value().commands);
                      });
    }

    void PostgresBlockIndex::indexAccountIdHeight(const std::string &account_id,
                                                  const std::string &height) {
      execute_(
          "INSERT INTO height_by_account_set(account_id, height) "
          "VALUES ("
          + transaction_.quote(account_id) + ", " + transaction_.quote(height)
          + ");");
    }

    void PostgresBlockIndex::indexAccountAssets(
        const std::string &account_id,
        const std::string &height,
        const std::string &index,
        const model::Transaction::CommandsType &commands) {
      using UserAssetsType =
          std::unordered_map<std::string, std::unordered_set<std::string>>;

      // flat map abstract commands to transfers
      auto transfers =
          commands | boost::adaptors::transformed([](const auto &cmd) {
            return std::dynamic_pointer_cast<model::TransferAsset>(cmd);
          })
          | boost::adaptors::filtered(
                [](const auto &cmd) { return bool(cmd); });

      boost::accumulate(
          transfers, UserAssetsType{}, [&](auto &&acc, const auto &cmd) {
            for (const auto &id : {cmd->src_account_id, cmd->dest_account_id}) {
              this->indexAccountIdHeight(id, height);
            }

            auto ids = {account_id, cmd->src_account_id, cmd->dest_account_id};
            // flat map accounts to unindexed keys
            boost::for_each(ids, [&](const auto &id) {
              execute_(
                  "INSERT INTO index_by_id_height_asset(id, height, asset_id, "
                  "index) "
                  "VALUES ("
                  + transaction_.quote(id) + ", " + transaction_.quote(height)
                  + ", " + transaction_.quote(cmd->asset_id) + ", "
                  + transaction_.quote(index) + ");");
            });
            return std::forward<decltype(acc)>(acc);
          });
    }
  }  // namespace ametsuchi
}  // namespace iroha
