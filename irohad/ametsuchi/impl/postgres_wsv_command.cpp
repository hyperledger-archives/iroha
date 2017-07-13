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

#include "ametsuchi/impl/postgres_wsv_command.hpp"

namespace iroha {
  namespace ametsuchi {

    bool PostgresWsvCommand::upsertAccount(const model::Account &account) {
      return false;
    }

    bool PostgresWsvCommand::insertAsset(const model::Asset &asset) {
      return false;
    }

    bool PostgresWsvCommand::upsertAccountAsset(
        const model::AccountAsset &asset) {
      return false;
    }

    bool PostgresWsvCommand::insertSignatory(
        const ed25519::pubkey_t &signatory) {
      return false;
    }

    bool PostgresWsvCommand::insertAccountSignatory(
        const std::string &account_id, const ed25519::pubkey_t &signatory) {
      return false;
    }

    bool PostgresWsvCommand::deleteAccountSignatory(
        const std::string &account_id, const ed25519::pubkey_t &signatory) {
      return false;
    }

    bool PostgresWsvCommand::upsertPeer(const model::Peer &peer) {
      return false;
    }

    bool PostgresWsvCommand::deletePeer(const model::Peer &peer) {
      return false;
    }

    bool PostgresWsvCommand::insertDomain(const model::Domain &domain) {
      return false;
    }

    PostgresWsvCommand::PostgresWsvCommand(
        std::unique_ptr<pqxx::nontransaction> &transaction)
        : transaction_(transaction) {}
  }  // namespace ametsuchi
}  // namespace iroha