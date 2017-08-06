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

#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/assign_master_key.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"
#include "model/commands/remove_signatory.hpp"
#include "model/commands/set_permissions.hpp"
#include "model/commands/set_quorum.hpp"
#include "model/commands/subtract_asset_quantity.hpp"
#include "model/commands/transfer_asset.hpp"

#include <math.h>
#include <cmath>

namespace iroha {
  namespace model {

    bool AddAssetQuantity::execute(ametsuchi::WsvQuery &queries,
                                   ametsuchi::WsvCommand &commands) {
      auto asset = queries.getAsset(asset_id);
      if (!asset)
        // No such asset
        return false;
      auto precision = asset.value().precision;
      // Amount is wrongly formed
      if (amount.get_frac_number() > precision) return false;
      if (!queries.getAccount(account_id))
        // No such account
        return false;
      auto account_asset = queries.getAccountAsset(account_id, asset_id);
      AccountAsset accountAsset;
      // Such accountAsset not found
      if (!account_asset) {
        // No wallet found -> create new
        accountAsset = AccountAsset();
        accountAsset.asset_id = asset_id;
        accountAsset.account_id = account_id;
        accountAsset.balance = amount.get_joint_amount(precision);
      } else {
        accountAsset = account_asset.value();
        // TODO: handle non trivial arithmetic
        auto new_balance =
            account_asset.value().balance + amount.get_joint_amount(precision);
        // TODO: handle overflow
        accountAsset.balance = new_balance;
      }

      // accountAsset.value().balance += amount;
      return commands.upsertAccountAsset(accountAsset);
    }

    bool AddPeer::execute(ametsuchi::WsvQuery &queries,
                          ametsuchi::WsvCommand &commands) {
      Peer peer;
      peer.address = address;
      peer.pubkey = peer_key;
      // Will return false if peer is not unique
      return commands.insertPeer(peer);
    }

    bool AddSignatory::execute(ametsuchi::WsvQuery &queries,
                               ametsuchi::WsvCommand &commands) {
      return commands.insertAccountSignatory(account_id, pubkey);
    }

    bool AssignMasterKey::execute(ametsuchi::WsvQuery &queries,
                                  ametsuchi::WsvCommand &commands) {
      auto account = queries.getAccount(account_id);
      if (!account)
        // Such account not found
        return false;
      account.value().master_key = pubkey;
      return commands.updateAccount(account.value());
    }

    bool CreateAccount::execute(ametsuchi::WsvQuery &queries,
                                ametsuchi::WsvCommand &commands) {
      Account account;
      account.master_key = pubkey;
      account.account_id = account_name + "@" + domain_id;

      account.domain_name = domain_id;
      account.quorum = 1;
      Account::Permissions permissions = iroha::model::Account::Permissions();
      account.permissions = permissions;

      return commands.insertSignatory(pubkey) &&
             commands.insertAccount(account) &&
             commands.insertAccountSignatory(account.account_id, pubkey);
    }

    bool CreateAsset::execute(ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands) {
      Asset new_asset;
      new_asset.asset_id = asset_name + "#" + domain_id;
      new_asset.domain_id = domain_id;
      new_asset.precision = precision;
      // The insert will fail if asset already exist
      return commands.insertAsset(new_asset);
    }

    bool CreateDomain::execute(ametsuchi::WsvQuery &queries,
                               ametsuchi::WsvCommand &commands) {
      Domain new_domain;
      new_domain.domain_id = domain_name;
      // The insert will fail if domain already exist
      return commands.insertDomain(new_domain);
    }

    bool RemoveSignatory::execute(ametsuchi::WsvQuery &queries,
                                  ametsuchi::WsvCommand &commands) {
      // Delete will fail if account signatory doesn't exist
      return commands.deleteAccountSignatory(account_id, pubkey);
    }

    bool SetAccountPermissions::execute(ametsuchi::WsvQuery &queries,
                                        ametsuchi::WsvCommand &commands) {
      auto account = queries.getAccount(account_id);
      if (!account)
        // There is no such account
        return false;

      account.value().permissions = new_permissions;
      return commands.updateAccount(account.value());
    }

    bool SetQuorum::execute(ametsuchi::WsvQuery &queries,
                            ametsuchi::WsvCommand &commands) {
      auto account = queries.getAccount(account_id);
      if (!account)
        // There is no such account
        return false;

      account.value().quorum = new_quorum;
      return commands.updateAccount(account.value());
    }

    bool TransferAsset::execute(ametsuchi::WsvQuery &queries,
                                ametsuchi::WsvCommand &commands) {
      auto src_account_asset =
          queries.getAccountAsset(src_account_id, asset_id);
      if (!src_account_asset) {
        // There is no src AccountAsset
        return false;
      }

      AccountAsset dest_AccountAsset;
      auto dest_account_asset =
          queries.getAccountAsset(dest_account_id, asset_id);
      auto asset = queries.getAsset(asset_id);
      if (!asset)
        // No asset found
        return false;
      // Precision for both wallets
      auto precision = asset.value().precision;
      if (amount.get_frac_number() > precision)
        // Precision is wrong
        return false;
      // Get src balance
      auto src_balance = src_account_asset.value().balance;
      // TODO: handle non-trivial arithmetic
      src_balance -= amount.get_joint_amount(precision);
      // Set new balance for source account
      src_account_asset.value().balance = src_balance;

      if (!dest_account_asset) {
        // This assert is new for this account - create new AccountAsset
        dest_AccountAsset = AccountAsset();
        dest_AccountAsset.asset_id = asset_id;
        dest_AccountAsset.account_id = dest_account_id;
        // Set new balance for dest account
        dest_AccountAsset.balance = amount.get_joint_amount(precision);

      } else {
        // Account already has such asset
        dest_AccountAsset = dest_account_asset.value();
        // Get balance dest account
        auto dest_balance = dest_account_asset.value().balance;

        dest_balance += amount.get_joint_amount(precision);
        // Set new balance for dest
        dest_AccountAsset.balance = dest_balance;
      }

      return commands.upsertAccountAsset(dest_AccountAsset) &&
             commands.upsertAccountAsset(src_account_asset.value());
    }
  }
}
