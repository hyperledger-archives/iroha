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

#include "ametsuchi/impl/mutable_storage_impl.hpp"
#include <model/commands/transfer_asset.hpp>

#include "ametsuchi/impl/postgres_wsv_command.hpp"
#include "ametsuchi/impl/postgres_wsv_query.hpp"

#include "crypto/hash.hpp"

namespace iroha {
  namespace ametsuchi {
    MutableStorageImpl::MutableStorageImpl(
        hash256_t top_hash,
        std::unique_ptr<cpp_redis::redis_client> index,
        std::unique_ptr<pqxx::lazyconnection> connection,
        std::unique_ptr<pqxx::nontransaction> transaction,
        std::shared_ptr<model::CommandExecutorFactory> command_executors)
        : top_hash_(top_hash),
          index_(std::move(index)),
          connection_(std::move(connection)),
          transaction_(std::move(transaction)),
          wsv_(std::make_unique<PostgresWsvQuery>(*transaction_)),
          executor_(std::make_unique<PostgresWsvCommand>(*transaction_)),
          command_executors_(std::move(command_executors)),
          committed(false) {
      index_->multi();
      transaction_->exec("BEGIN;");
    }

    void MutableStorageImpl::index_block(uint64_t height, model::Block block) {
      for (size_t i = 0; i < block.transactions.size(); i++) {
        auto tx = block.transactions.at(i);
        auto account_id = tx.creator_account_id;
        auto hash = iroha::hash(tx).to_hexstring();

        // tx hash -> block where hash is stored
        index_->set(hash, std::to_string(height));

        // to make index account_id -> list of blocks where his txs exist
        index_->rpush(account_id, {std::to_string(height)});

        // to make index account_id:height -> list of tx indexes (where
        // tx is placed in the block)
        index_->rpush(account_id + ":" + std::to_string(height),
                      {std::to_string(i)});

        // collect all assets belonging to user "account_id"
        std::set<std::string> users_assets_in_tx;
        std::for_each(tx.commands.begin(),
                      tx.commands.end(),
                      [&account_id, &users_assets_in_tx](auto command) {
                        if (instanceof <model::TransferAsset>(*command)) {
                          auto transferAsset =
                              (model::TransferAsset *)command.get();
                          if (transferAsset->dest_account_id == account_id
                              or transferAsset->src_account_id == account_id) {
                            users_assets_in_tx.insert(transferAsset->asset_id);
                          }
                        }
                      });

        // to make account_id:height:asset_id -> list of tx indexes (where tx
        // with certain asset is placed in the block )
        for (const auto &asset_id : users_assets_in_tx) {
          // create key to put user's txs with given asset_id
          std::string account_assets_key;
          account_assets_key.append(account_id);
          account_assets_key.append(":");
          account_assets_key.append(std::to_string(height));
          account_assets_key.append(":");
          account_assets_key.append(asset_id);
          index_->rpush(account_assets_key, {std::to_string(i)});
        }
      }
    }

    bool MutableStorageImpl::apply(
        const model::Block &block,
        std::function<bool(const model::Block &, WsvQuery &, const hash256_t &)>
            function) {
      auto execute_command = [this](auto command) {
        return command_executors_->getCommandExecutor(command)->execute(
            *command, *wsv_, *executor_);
      };
      auto execute_transaction = [this, execute_command](auto &transaction) {
        return std::all_of(transaction.commands.begin(),
                           transaction.commands.end(),
                           execute_command);
      };

      transaction_->exec("SAVEPOINT savepoint_;");
      auto result = function(block, *wsv_, top_hash_)
          and std::all_of(block.transactions.begin(),
                          block.transactions.end(),
                          execute_transaction);

      if (result) {
        block_store_.insert(std::make_pair(block.height, block));
        index_block(block.height, block);

        top_hash_ = block.hash;
        transaction_->exec("RELEASE SAVEPOINT savepoint_;");
      } else {
        transaction_->exec("ROLLBACK TO SAVEPOINT savepoint_;");
      }
      return result;
    }

    MutableStorageImpl::~MutableStorageImpl() {
      if (not committed) {
        index_->discard();
        transaction_->exec("ROLLBACK;");
      }
    }
  }  // namespace ametsuchi
}  // namespace iroha
