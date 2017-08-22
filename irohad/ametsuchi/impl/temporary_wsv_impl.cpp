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

#include <ametsuchi/impl/temporary_wsv_impl.hpp>

#include <algorithm>

namespace iroha {
  namespace ametsuchi {

    bool TemporaryWsvImpl::apply(
        const model::Transaction &transaction,
        std::function<bool(const model::Transaction &, WsvQuery &)> function) {
      auto execute_command = [this, transaction](auto command) {
        auto executor = command_executors_->getCommandExecutor(command);
        auto account = this->getAccount(transaction.creator_account_id).value();
        return executor->validate(*command, *this, account) &&
            executor->execute(*command, *this, *executor_);
      };

      transaction_->exec("SAVEPOINT savepoint_;");
      auto result = function(transaction, *this) &&
          std::all_of(transaction.commands.begin(),
                      transaction.commands.end(), execute_command);
      if (result) {
        transaction_->exec("RELEASE SAVEPOINT savepoint_;");
      } else {
        transaction_->exec("ROLLBACK TO SAVEPOINT savepoint_;");
      }
      return result;
    }

    TemporaryWsvImpl::TemporaryWsvImpl(
        std::unique_ptr<pqxx::lazyconnection> connection,
        std::unique_ptr<pqxx::nontransaction> transaction,
        std::unique_ptr<WsvQuery> wsv, std::unique_ptr<WsvCommand> executor,
        std::shared_ptr<model::CommandExecutorFactory> command_executors)
        : connection_(std::move(connection)),
          transaction_(std::move(transaction)),
          wsv_(std::move(wsv)),
          executor_(std::move(executor)),
          command_executors_(std::move(command_executors)) {
      transaction_->exec("BEGIN;");
    }

    TemporaryWsvImpl::~TemporaryWsvImpl() { transaction_->exec("ROLLBACK;"); }

    nonstd::optional<model::Account> TemporaryWsvImpl::getAccount(
        const std::string &account_id) {
      return wsv_->getAccount(account_id);
    }

    nonstd::optional<std::vector<ed25519::pubkey_t>>
    TemporaryWsvImpl::getSignatories(const std::string &account_id) {
      return wsv_->getSignatories(account_id);
    }

    nonstd::optional<model::Asset> TemporaryWsvImpl::getAsset(
        const std::string &asset_id) {
      return wsv_->getAsset(asset_id);
    }

    nonstd::optional<model::AccountAsset> TemporaryWsvImpl::getAccountAsset(
        const std::string &account_id, const std::string &asset_id) {
      return wsv_->getAccountAsset(account_id, asset_id);
    }

    nonstd::optional<std::vector<model::Peer>> TemporaryWsvImpl::getPeers() {
      return wsv_->getPeers();
    }

  }  // namespace ametsuchi
}  // namespace iroha
