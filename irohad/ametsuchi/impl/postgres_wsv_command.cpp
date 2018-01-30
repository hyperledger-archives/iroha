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

#include <boost/format.hpp>

#include "ametsuchi/impl/postgres_wsv_command.hpp"

namespace iroha {
  namespace ametsuchi {

    PostgresWsvCommand::PostgresWsvCommand(pqxx::nontransaction &transaction)
        : transaction_(transaction),
          log_(logger::log("PostgresWsvCommand")),
          execute_{makeExecute(transaction_, log_)} {}

    WsvCommandResult PostgresWsvCommand::insertRole(
        const std::string &role_name) {
      auto result = execute_("INSERT INTO role(role_id) VALUES ("
                             + transaction_.quote(role_name) + ");");

      auto message =
          (boost::format("failed to insert role: '%s'") % role_name).str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertAccountRole(
        const std::string &account_id, const std::string &role_name) {
      auto result =
          execute_("INSERT INTO account_has_roles(account_id, role_id) VALUES ("
                   + transaction_.quote(account_id) + ", "
                   + transaction_.quote(role_name) + ");");

      auto message =
          (boost::format(
               "failed to insert account role, account: '%s', role name: '%s'")
           % account_id % role_name)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::deleteAccountRole(
        const std::string &account_id, const std::string &role_name) {
      auto result = execute_("DELETE FROM account_has_roles WHERE account_id="
                             + transaction_.quote(account_id) + "AND role_id="
                             + transaction_.quote(role_name) + ";");
      auto message =
          (boost::format(
               "failed to delete account role, account id: %s, role name: '%s'")
           % account_id % role_name)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertRolePermissions(
        const std::string &role_id, const std::set<std::string> &permissions) {
      auto entry = [this, &role_id](auto permission) {
        return "(" + transaction_.quote(role_id) + ", "
            + transaction_.quote(permission) + ")";
      };

      auto result = execute_(
          "INSERT INTO role_has_permissions(role_id, permission_id) VALUES "
          + std::accumulate(
                std::next(permissions.begin()),
                permissions.end(),
                entry(*permissions.begin()),
                [&entry](auto acc, auto x) { return acc + ", " + entry(x); })
          + ";");

      auto all_permissions =
          std::accumulate(std::next(permissions.begin()),
                          permissions.end(),
                          permissions.begin(),
                          [](auto res, auto b) { return res + ", " + b; });

      auto message = (boost::format("failed to insert role permissions, role "
                                    "id: '%s', permissions: %s")
                      % role_id % all_permissions)
                         .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertAccountGrantablePermission(
        const std::string &permittee_account_id,
        const std::string &account_id,
        const std::string &permission_id) {
      auto result = execute_(
          "INSERT INTO "
          "account_has_grantable_permissions(permittee_account_id, "
          "account_id, permission_id) VALUES ("
          + transaction_.quote(permittee_account_id) + ", "
          + transaction_.quote(account_id) + ", "
          + transaction_.quote(permission_id) + ");");

      auto message =
          (boost::format("failed to insert account grantable permission, "
                         "permittee account id: '%s', "
                         "account id: '%s', "
                         "permission id: '%s'")
           % permittee_account_id % account_id % permission_id)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::deleteAccountGrantablePermission(
        const std::string &permittee_account_id,
        const std::string &account_id,
        const std::string &permission_id) {
      auto result = execute_(
          "DELETE FROM public.account_has_grantable_permissions WHERE "
          "permittee_account_id="
          + transaction_.quote(permittee_account_id)
          + " AND account_id=" + transaction_.quote(account_id)
          + " AND permission_id=" + transaction_.quote(permission_id) + " ;");

      auto message =
          (boost::format("failed to delete account grantable permission, "
                         "permittee account id: '%s', "
                         "account id: '%s', "
                         "permission id: '%s'")
           % permittee_account_id % account_id % permission_id)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertAccount(
        const model::Account &account) {
      auto result = execute_(
          "INSERT INTO account(account_id, domain_id, quorum, "
          "transaction_count, data) VALUES ("
          + transaction_.quote(account.account_id) + ", "
          + transaction_.quote(account.domain_id) + ", "
          + transaction_.quote(account.quorum)
          + ", "
          // Transaction counter
          + transaction_.quote(default_tx_counter) + ", "
          + transaction_.quote(account.json_data) + ");");

      auto message = (boost::format("failed to insert account, "
                                    "account id: '%s', "
                                    "domain id: '%s', "
                                    "quorum: '%d', "
                                    "transaction counter: '%d', "
                                    "json_data: %s")
                      % account.account_id % account.domain_id % account.quorum
                      % default_tx_counter % account.json_data)
                         .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertAsset(
        const model::Asset &asset) {
      uint32_t precision = asset.precision;
      auto result = execute_(
          "INSERT INTO asset(asset_id, domain_id, \"precision\", data) "
          "VALUES ("
          + transaction_.quote(asset.asset_id) + ", "
          + transaction_.quote(asset.domain_id) + ", "
          + transaction_.quote(precision) + ", " + /*asset.data*/ "NULL"
          + ");");

      auto message = (boost::format("failed to insert asset, asset id: '%s', "
                                    "domain id: '%s', precision: %d")
                      % asset.asset_id % asset.domain_id % precision)
                         .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::upsertAccountAsset(
        const model::AccountAsset &asset) {
      auto result = execute_(
            "INSERT INTO account_has_asset(account_id, asset_id, amount) "
            "VALUES ("
            + transaction_.quote(asset.account_id) + ", "
            + transaction_.quote(asset.asset_id) + ", "
            + transaction_.quote(asset.balance.to_string())
            + ") ON CONFLICT (account_id, asset_id) DO UPDATE SET "
            "amount = EXCLUDED.amount;");

      auto message =
          (boost::format("failed to upsert account, account id: '%s', "
                         "asset id: '%s', balance: %s")
           % asset.account_id % asset.asset_id % asset.balance.to_string())
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertSignatory(
        const pubkey_t &signatory) {
      auto result = execute_("INSERT INTO signatory(public_key) VALUES ("
                             + transaction_.quote(pqxx::binarystring(
                                   signatory.data(), signatory.size()))
                             + ") ON CONFLICT DO NOTHING;");

      auto message =
          (boost::format("failed to insert signatory, hex string: '%s'")
           % signatory.to_hexstring())
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertAccountSignatory(
        const std::string &account_id, const pubkey_t &signatory) {
      auto result = execute_(
          "INSERT INTO account_has_signatory(account_id, public_key) VALUES ("
          + transaction_.quote(account_id) + ", "
          + transaction_.quote(
                pqxx::binarystring(signatory.data(), signatory.size()))
          + ");");

      auto message =
          (boost::format("failed to insert account signatory, account id: "
                         "'%s', signatory hex string: '%s")
           % account_id % signatory.to_hexstring())
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::deleteAccountSignatory(
        const std::string &account_id, const pubkey_t &signatory) {
      auto result =
          execute_("DELETE FROM account_has_signatory WHERE account_id = "
                   + transaction_.quote(account_id) + " AND public_key = "
                   + transaction_.quote(
                         pqxx::binarystring(signatory.data(), signatory.size()))
                   + ";");

      auto message =
          (boost::format("failed to delete account signatory, account id: "
                         "'%s', signatory hex string: '%s'"))
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::deleteSignatory(
        const pubkey_t &signatory) {
      pqxx::binarystring public_key(signatory.data(), signatory.size());
      auto result = execute_("DELETE FROM signatory WHERE public_key = "
                    + transaction_.quote(public_key)
                    + " AND NOT EXISTS (SELECT 1 FROM account_has_signatory "
                        "WHERE public_key = "
                    + transaction_.quote(public_key)
                    + ") AND NOT EXISTS (SELECT 1 FROM peer WHERE public_key = "
                    + transaction_.quote(public_key) + ");");

      auto message =
          (boost::format("failed to delete signatory, hex string: '%s'")
           % signatory.to_hexstring())
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertPeer(const model::Peer &peer) {
      auto result = execute_("INSERT INTO peer(public_key, address) VALUES ("
                             + transaction_.quote(pqxx::binarystring(
                                   peer.pubkey.data(), peer.pubkey.size()))
                             + ", " + transaction_.quote(peer.address) + ");");

      auto message =
          (boost::format(
               "failed to insert peer, public key: '%s', address: '%s'")
           % peer.pubkey.to_hexstring() % peer.address)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::deletePeer(const model::Peer &peer) {
      auto result = execute_("DELETE FROM peer WHERE public_key = "
                             + transaction_.quote(pqxx::binarystring(
                                   peer.pubkey.data(), peer.pubkey.size()))
                             + " AND address = "
                             + transaction_.quote(peer.address) + ";");

      auto message =
          (boost::format(
               "failed to delete peer, public key: '%s', address: '%s'")
           % peer.pubkey.to_hexstring() % peer.address)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::insertDomain(
        const model::Domain &domain) {
      auto result =
          execute_("INSERT INTO domain(domain_id, default_role) VALUES ("
                   + transaction_.quote(domain.domain_id) + ", "
                   + transaction_.quote(domain.default_role) + ");");

      auto message =
          (boost::format(
               "failed to insert domain, domain id: '%s', default role: '%s'")
           % domain.domain_id % domain.default_role)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::updateAccount(
        const model::Account &account) {
      auto result = execute_(
            "UPDATE account\n"
            "   SET quorum=" +
            transaction_.quote(account.quorum) +
            ", transaction_count=" +
            /*account.transaction_count*/ transaction_.quote(default_tx_counter) +
            "\n"
            " WHERE account_id=" +
            transaction_.quote(account.account_id) + ";");

      auto message =
          (boost::format(
               "failed to update account, account id: '%s', quorum: '%s'")
           % account.account_id % account.quorum)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::setAccountKV(
        const std::string &account_id,
        const std::string &creator_account_id,
        const std::string &key,
        const std::string &val) {
      auto result = execute_(
          "UPDATE account SET data = jsonb_set(CASE WHEN data ?"
          + transaction_.quote(creator_account_id)
          + " THEN data ELSE jsonb_set(data, "
          + transaction_.quote("{" + creator_account_id + "}") + ","
          + transaction_.quote("{}") + ") END,"
          + transaction_.quote("{" + creator_account_id + ", " + key + "}")
          + "," + transaction_.quote("\"" + val + "\"")
          + ") WHERE account_id=" + transaction_.quote(account_id) + ";");

      auto message =
          (boost::format("failed to set account key-value, account id: '%s', "
                         "creator account id: '%s',\n key: '%s', value: '%s'")
           % account_id % creator_account_id % key % val)
              .str();

      return makeCommandResult(result, message);
    }

    WsvCommandResult PostgresWsvCommand::makeCommandResult(
        expected::Result<pqxx::result, std::string> result,
        const std::string &error_message) noexcept {
      return result.match([](expected::Value<pqxx::result> v) { return {}; },
                          [&error_message](expected::Error<std::string> e) {
                            return expected::makeError(error_message + e.error);
                          });
    }
  }  // namespace ametsuchi
}  // namespace iroha
