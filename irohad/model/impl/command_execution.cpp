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

namespace iroha {
  namespace model {

    bool AddAssetQuantity::execute(ametsuchi::WsvQuery &queries,
                                   ametsuchi::WsvCommand &commands) {
      auto accountAsset = queries.getAccountAsset(account_id, asset_id);
      // Such accountAsset not found
      if (!accountAsset) return false;
      accountAsset.value().balance += amount;
      return commands.upsertAccountAsset(accountAsset.value());
    }

    bool AddPeer::execute(ametsuchi::WsvQuery &queries,
                          ametsuchi::WsvCommand &commands) {
      Peer peer;
      peer.address = address;
      peer.pubkey = peer_key;
      return commands.upsertPeer(peer);
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
      return commands.upsertAccount(account.value());
    }

    bool CreateAccount::execute(ametsuchi::WsvQuery &queries,
                                ametsuchi::WsvCommand &commands) {
      Account account;
      account.master_key = pubkey;
      account.account_id = account_name + "@" + domain_id;
      if (queries.getAccount(account.account_id))
        // Such account already exist
        return false;

      account.domain_name = domain_id;
      account.quorum = 1;
      Account::Permissions permissions = iroha::model::Account::Permissions();
      account.permissions = permissions;

      return commands.insertSignatory(pubkey) &&
             commands.upsertAccount(account) &&
             commands.insertAccountSignatory(account.account_id, pubkey);
    }

    bool CreateAsset::execute(ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands) {
      Asset new_asset;
      new_asset.name = asset_name + "#" + domain_id;
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
      return commands.upsertAccount(account.value());
    }

    bool SetQuorum::execute(ametsuchi::WsvQuery &queries,
                            ametsuchi::WsvCommand &commands) {
      auto account = queries.getAccount(account_id);
      if (!account)
        // There is no such account
        return false;

      account.value().quorum = new_quorum;
      return commands.upsertAccount(account.value());
    }

    bool TransferAsset::execute(ametsuchi::WsvQuery &queries,
                                ametsuchi::WsvCommand &commands) {
      auto src_account_assert =
          queries.getAccountAsset(src_account_id, asset_id);
      if (!src_account_assert) {
        // There is no src AccountAsset
        return false;
      }

      AccountAsset dest_AccountAssert;
      auto dest_account_assert =
          queries.getAccountAsset(dest_account_id, asset_id);
      if (!dest_account_assert) {
        // This assert is new for this account - create new AccountAsset
        dest_AccountAssert = AccountAsset();
        dest_AccountAssert.asset_id = asset_id;
        dest_AccountAssert.account_id = dest_account_id;
        dest_AccountAssert.balance = amount;
      } else {
        // Account already has such asset
        dest_AccountAssert = dest_account_assert.value();
        dest_AccountAssert.balance += amount;
        src_account_assert.value().balance -= amount;
      }

      return commands.upsertAccountAsset(dest_AccountAssert) &&
             commands.upsertAccountAsset(src_account_assert.value());
    }
  }
}