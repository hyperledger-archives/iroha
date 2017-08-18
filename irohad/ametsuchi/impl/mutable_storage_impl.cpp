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

#include <algorithm>

#include <ametsuchi/impl/mutable_storage_impl.hpp>

namespace iroha {
  namespace ametsuchi {

    bool MutableStorageImpl::apply(
        const model::Block &block,
        std::function<bool(const model::Block &, WsvQuery &, const hash256_t &)>
            function) {
      auto execute_command = [this](auto command) {
        return command_executors_->getCommandExecutor(command)->execute(
            *command, *this, *executor_);
      };
      auto execute_transaction = [this, execute_command](auto &transaction) {
        return std::all_of(transaction.commands.begin(),
                           transaction.commands.end(), execute_command);
      };

      transaction_->exec("SAVEPOINT savepoint_;");
      auto result = function(block, *this, top_hash_) &&
                    std::all_of(block.transactions.begin(),
                                block.transactions.end(), execute_transaction);

      if (result) {
        block_store_.insert(std::make_pair(block.height, block));
        top_hash_ = block.hash;
        transaction_->exec("RELEASE SAVEPOINT savepoint_;");
      } else {
        transaction_->exec("ROLLBACK TO SAVEPOINT savepoint_;");
      }
      return result;
    }

    MutableStorageImpl::MutableStorageImpl(
        hash256_t top_hash, std::unique_ptr<cpp_redis::redis_client> index,
        std::unique_ptr<pqxx::lazyconnection> connection,
        std::unique_ptr<pqxx::nontransaction> transaction,
        std::unique_ptr<WsvQuery> wsv, std::unique_ptr<WsvCommand> executor,
        std::shared_ptr<model::CommandExecutorFactory> command_executors)
        : top_hash_(top_hash),
          index_(std::move(index)),
          connection_(std::move(connection)),
          transaction_(std::move(transaction)),
          wsv_(std::move(wsv)),
          executor_(std::move(executor)),
          command_executors_(std::move(command_executors)),
          committed(false) {
      index_->multi();
      transaction_->exec("BEGIN;");
    }

    MutableStorageImpl::~MutableStorageImpl() {
      if (!committed) {
        index_->discard();
        transaction_->exec("ROLLBACK;");
      }
    }

    nonstd::optional<model::Account> MutableStorageImpl::getAccount(
        const std::string &account_id) {
      return wsv_->getAccount(account_id);
    }

    nonstd::optional<std::vector<ed25519::pubkey_t>>
    MutableStorageImpl::getSignatories(const std::string &account_id) {
      return wsv_->getSignatories(account_id);
    }

    nonstd::optional<model::Asset> MutableStorageImpl::getAsset(
        const std::string &asset_id) {
      return wsv_->getAsset(asset_id);
    }

    nonstd::optional<model::AccountAsset> MutableStorageImpl::getAccountAsset(
        const std::string &account_id, const std::string &asset_id) {
      return wsv_->getAccountAsset(account_id, asset_id);
    }

    nonstd::optional<std::vector<model::Peer>> MutableStorageImpl::getPeers() {
      return wsv_->getPeers();
    }
  }  // namespace ametsuchi
}  // namespace iroha
