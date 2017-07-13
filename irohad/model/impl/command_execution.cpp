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

    /**
     *
     * @param queries
     * @param commands
     * @return
     */
    bool AddAssetQuantity::execute(ametsuchi::WsvQuery &queries,
                                   ametsuchi::WsvCommand &commands) {
      auto accountAsset = queries.getAccountAsset(account_id, asset_id);
      // Such accountAsset not found
      if (!accountAsset) return false;
      // TODO: make +=
      accountAsset.value().balance = amount;
      return commands.upsertAccountAsset(accountAsset.value());
    }

    /**
     *
     * @param queries
     * @param commands
     * @return
     */
    bool AddSignatory::execute(ametsuchi::WsvQuery &queries,
                               ametsuchi::WsvCommand &commands) {
      return commands.insertAccountSignatory(account_id, pubkey);
    }

    /**
     *
     * @param queries
     * @param commands
     * @return
     */
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
      account.account_id = domain_id + "@" + account_name;
      if (queries.getAccount(account.account_id))
        // Such account already exist
        return false;

      account.domain_name = domain_id;
      account.quorum = 1;
      Account::Permissions permissions = iroha::model::Account::Permissions();
      account.permissions = permissions;

      return commands.upsertAccount(account);
    }

    bool CreateAsset::execute(ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands) {
      Asset new_asset;
      new_asset.name = domain_id + "@" + asset_name;
      new_asset.precision = precision;
      // The insert will fail if asset already exist
      return commands.insertAsset(new_asset);
    }

  }
}