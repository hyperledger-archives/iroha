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
#include <iostream>

namespace iroha {
  namespace ametsuchi {

    bool PostgresWsvCommand::upsertAccount(const model::Account &account) {
      try {
        pqxx::binarystring master_key(account.master_key.data(),
                                      account.master_key.size());
        std::stringstream permissions;
        permissions << account.permissions.add_signatory
                    << account.permissions.can_transfer
                    << account.permissions.create_accounts
                    << account.permissions.create_assets
                    << account.permissions.create_domains
                    << account.permissions.issue_assets
                    << account.permissions.read_all_accounts
                    << account.permissions.remove_signatory
                    << account.permissions.set_permissions
                    << account.permissions.set_quorum;
        transaction_.exec(
            "INSERT INTO account(\n"
            "            account_id, domain_id, master_key, quorum, status, "
            "transaction_count, \n"
            "            permissions)\n"
            "    VALUES (" +
            transaction_.quote(account.account_id) + ", " +
            transaction_.quote(account.domain_name) + ", " +
            transaction_.quote(master_key) + ", " +
            transaction_.quote(account.quorum) + ", " +
            /*account.status*/ transaction_.quote(0) + ", " +
            /*account.transaction_count*/ transaction_.quote(0) +
            ", \n"
            "            " +
            transaction_.quote(permissions.str()) + ");");
      } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
      }
      return true;
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
      try {
        pqxx::binarystring public_key(signatory.data(), signatory.size());
        transaction_.exec(
            "INSERT INTO signatory(\n"
            "            public_key)\n"
            "    VALUES (" +
            transaction_.quote(public_key) + ");");
      } catch (const std::exception &e) {
        return false;
      }
      return true;
    }

    bool PostgresWsvCommand::insertAccountSignatory(
        const std::string &account_id, const ed25519::pubkey_t &signatory) {
      try {
        pqxx::binarystring public_key(signatory.data(), signatory.size());
        transaction_.exec(
            "INSERT INTO account_has_signatory(\n"
            "            account_id, public_key)\n"
            "    VALUES (" +
            transaction_.quote(account_id) + ", " +
            transaction_.quote(public_key) + ");");
      } catch (const std::exception &e) {
        return false;
      }
      return true;
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
      try {
        transaction_.exec(
            "INSERT INTO domain(\n"
            "            domain_id, open)\n"
            "    VALUES (" +
            transaction_.quote(domain.domain_id) + ", " +
            /*domain.open*/ transaction_.quote(true) + ");");
      } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
      }
      return true;
    }

    PostgresWsvCommand::PostgresWsvCommand(pqxx::nontransaction &transaction)
        : transaction_(transaction) {}
  }  // namespace ametsuchi
}  // namespace iroha