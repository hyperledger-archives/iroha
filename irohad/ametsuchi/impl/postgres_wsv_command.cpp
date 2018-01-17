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

    PostgresWsvCommand::PostgresWsvCommand(pqxx::nontransaction &transaction)
        : transaction_(transaction), log_(logger::log("PostgresWsvCommand")) {}

    bool PostgresWsvCommand::insertRole(const std::string &role_name) {
      return execute("INSERT INTO role(role_id) VALUES ("
                     + transaction_.quote(role_name) + ");");
    }

    bool PostgresWsvCommand::insertAccountRole(const std::string &account_id,
                                               const std::string &role_name) {
      return execute(
          "INSERT INTO account_has_roles(account_id, role_id) VALUES ("
          + transaction_.quote(account_id) + ", "
          + transaction_.quote(role_name) + ");");
    }

    bool PostgresWsvCommand::deleteAccountRole(const std::string &account_id,
                                               const std::string &role_name) {
      return execute("DELETE FROM account_has_roles WHERE account_id="
                     + transaction_.quote(account_id)
                     + "AND role_id=" + transaction_.quote(role_name) + ";");
    }

    bool PostgresWsvCommand::insertRolePermissions(
        const std::string &role_id, const std::set<std::string> &permissions) {
      auto entry = [this, &role_id](auto permission) {
        return "(" + transaction_.quote(role_id) + ", "
            + transaction_.quote(permission) + ")";
      };
      return execute(
          "INSERT INTO role_has_permissions(role_id, permission_id) VALUES "
          + std::accumulate(
                std::next(permissions.begin()),
                permissions.end(),
                entry(*permissions.begin()),
                [&entry](auto acc, auto x) { return acc + ", " + entry(x); })
          + ";");
    }

    bool PostgresWsvCommand::insertAccountGrantablePermission(
        const std::string &permittee_account_id,
        const std::string &account_id,
        const std::string &permission_id) {
      return execute(
          "INSERT INTO "
          "account_has_grantable_permissions(permittee_account_id, "
          "account_id, permission_id) VALUES ("
          + transaction_.quote(permittee_account_id) + ", "
          + transaction_.quote(account_id) + ", "
          + transaction_.quote(permission_id) + ");");
    }

    bool PostgresWsvCommand::deleteAccountGrantablePermission(
        const std::string &permittee_account_id,
        const std::string &account_id,
        const std::string &permission_id) {
      return execute(
          "DELETE FROM public.account_has_grantable_permissions WHERE "
          "permittee_account_id="
          + transaction_.quote(permittee_account_id)
          + " AND account_id=" + transaction_.quote(account_id)
          + " AND permission_id=" + transaction_.quote(permission_id) + " ;");
    }

    bool PostgresWsvCommand::insertAccount(const model::Account &account) {
      return execute(
          "INSERT INTO account(account_id, domain_id, quorum, "
          "transaction_count, data) VALUES ("
          + transaction_.quote(account.account_id) + ", "
          + transaction_.quote(account.domain_id) + ", "
          + transaction_.quote(account.quorum)
          + ", "
          // Transaction counter
          + transaction_.quote(default_tx_counter) + ", "
          + transaction_.quote(account.json_data) + ");");
    }

    bool PostgresWsvCommand::insertAsset(const model::Asset &asset) {
      uint32_t precision = asset.precision;
      return execute(
          "INSERT INTO asset(asset_id, domain_id, \"precision\", data) "
          "VALUES ("
          + transaction_.quote(asset.asset_id) + ", "
          + transaction_.quote(asset.domain_id) + ", "
          + transaction_.quote(precision) + ", " + /*asset.data*/ "NULL"
          + ");");
    }

    bool PostgresWsvCommand::upsertAccountAsset(
        const model::AccountAsset &asset) {
      return execute(
            "INSERT INTO account_has_asset(account_id, asset_id, amount) "
            "VALUES ("
            + transaction_.quote(asset.account_id) + ", "
            + transaction_.quote(asset.asset_id) + ", "
            + transaction_.quote(asset.balance.to_string())
            + ") ON CONFLICT (account_id, asset_id) DO UPDATE SET "
            "amount = EXCLUDED.amount;");
    }

    bool PostgresWsvCommand::insertSignatory(const pubkey_t &signatory) {
      return makeBinaryString(signatory) | [&](const auto &public_key) {
        return this->execute("INSERT INTO signatory(public_key) VALUES ("
                       + transaction_.quote(public_key)
                       + ") ON CONFLICT DO NOTHING;");
      };
    }

    bool PostgresWsvCommand::insertAccountSignatory(
        const std::string &account_id, const pubkey_t &signatory) {
      return makeBinaryString(signatory) | [&](const auto &public_key) {
        return this->execute(
            "INSERT INTO account_has_signatory(account_id, public_key) VALUES ("
            + transaction_.quote(account_id) + ", "
            + transaction_.quote(public_key) + ");");
      };
    }

    bool PostgresWsvCommand::deleteAccountSignatory(
        const std::string &account_id, const pubkey_t &signatory) {
      return makeBinaryString(signatory) | [&](const auto &public_key) {
        return this->execute("DELETE FROM account_has_signatory WHERE account_id = "
                       + transaction_.quote(account_id) + " AND public_key = "
                       + transaction_.quote(public_key) + ";");
      };
    }

    bool PostgresWsvCommand::deleteSignatory(const pubkey_t &signatory) {
      return makeBinaryString(signatory) | [&](const auto &public_key) {
        // TODO: implement with other queries
        return this->execute("DELETE FROM signatory WHERE public_key = "
                    + transaction_.quote(public_key)
                    + " AND NOT EXISTS (SELECT 1 FROM account_has_signatory "
                        "WHERE public_key = "
                    + transaction_.quote(public_key)
                    + ") AND NOT EXISTS (SELECT 1 FROM peer WHERE public_key = "
                    + transaction_.quote(public_key) + ");");
      };
    }

    bool PostgresWsvCommand::insertPeer(const model::Peer &peer) {
      return makeBinaryString(peer.pubkey) | [&](const auto &public_key) {
        return this->execute("INSERT INTO peer(public_key, address) VALUES ("
                       + transaction_.quote(public_key) + ", "
                       + transaction_.quote(peer.address) + ");");
      };
    }

    bool PostgresWsvCommand::deletePeer(const model::Peer &peer) {
      return makeBinaryString(peer.pubkey) | [&](const auto &public_key) {
        return this->execute("DELETE FROM peer WHERE public_key = "
                       + transaction_.quote(public_key) + " AND address = "
                       + transaction_.quote(peer.address) + ";");
      };
    }

    bool PostgresWsvCommand::insertDomain(const model::Domain &domain) {
      return execute("INSERT INTO domain(domain_id, default_role) VALUES ("
                     + transaction_.quote(domain.domain_id) + ", "
                     + transaction_.quote(domain.default_role) + ");");
    }

    bool PostgresWsvCommand::updateAccount(const model::Account &account) {
      return execute(
            "UPDATE account\n"
            "   SET quorum=" +
            transaction_.quote(account.quorum) +
            ", transaction_count=" +
            /*account.transaction_count*/ transaction_.quote(default_tx_counter) +
            "\n"
            " WHERE account_id=" +
            transaction_.quote(account.account_id) + ";");
    }

    bool PostgresWsvCommand::setAccountKV(const std::string &account_id,
                                          const std::string &creator_account_id,
                                          const std::string &key,
                                          const std::string &val) {
      return execute(
          "UPDATE account SET data = jsonb_set(CASE WHEN data ?"
          + transaction_.quote(creator_account_id)
          + " THEN data ELSE jsonb_set(data, "
          + transaction_.quote("{" + creator_account_id + "}") + ","
          + transaction_.quote("{}") + ") END,"
          + transaction_.quote("{" + creator_account_id + ", " + key + "}")
          + "," + transaction_.quote("\"" + val + "\"")
          + ") WHERE account_id=" + transaction_.quote(account_id) + ";");
    }

    bool PostgresWsvCommand::execute(const std::string &statement) noexcept {
      try {
        transaction_.exec(statement);
      } catch (const std::exception &e) {
        log_->error(e.what());
        return false;
      }
      return true;
    }

    nonstd::optional<pqxx::binarystring> PostgresWsvCommand::makeBinaryString(
        const blob_t<32> &data) noexcept {
      try {
        return pqxx::binarystring(data.data(), data.size());
      } catch (const std::exception &e) {
        return nonstd::nullopt;
      }
    }
  }  // namespace ametsuchi
}  // namespace iroha
