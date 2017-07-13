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

#include <algorithm>

namespace iroha {
  namespace model {

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool AddAssetQuantity::validate(ametsuchi::WsvQuery &queries,
                                    const Account &creator) {
      // TODO: We will get rid off ugly string to float conversion once we have
      // Decimal abstraction
      try {
        // Amount must be in some meaningful range:
        if (std::stod(amount) > 0) return false;
      }
      // Check if asset is formed right
      catch (const std::invalid_argument &ia) {
        return false;
      }
      // Check if asset is in range
      catch (const std::out_of_range &oor) {
        return false;
      }

      // Check if creator has MoneyCreator permission
      return creator.permissions.issue_assets;
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool AddSignatory::validate(ametsuchi::WsvQuery &queries,
                                const Account &creator) {
      return
          // Case 1. When command creator wants to add their signatory to some
          // account
          creator.master_key == pubkey;
      // TODO: What about other cases ?
    }
    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool AssignMasterKey::validate(ametsuchi::WsvQuery &queries,
                                   const Account &creator) {
      // Case 1: When creator wants to change key in their own account
      auto signs = queries.getSignatories(account_id);

      return
          // Check if accout has at lest one signatory
          !signs.empty() &&
          // Check if new master key is not the same
          creator.master_key != pubkey &&
          // Check if new master key is in AccountSignatory relationship
          std::find(signs.begin(), signs.end(), pubkey) != signs.end();

      // TODO:Can there be case when creator can assign master key of other
      // account?
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool CreateAccount::validate(ametsuchi::WsvQuery &queries,
                                 const Account &creator) {
      // Creator must have permission to create account
      return creator.permissions.create_assets;
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool CreateAsset::validate(ametsuchi::WsvQuery &queries,
                               const Account &creator) {
      // Creator must have permission to create assets
      return creator.permissions.create_assets;
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool CreateDomain::validate(ametsuchi::WsvQuery &queries,
                                const Account &creator) {
      // Creator must have permission to create domains
      return creator.permissions.create_domains;
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool RemoveSignatory::validate(ametsuchi::WsvQuery &queries,
                                   const Account &creator) {
      // Case 1. Creator removes signatory from their account
      return creator.account_id == account_id &&
             // Can't remove master key (first reassign it)
             pubkey != creator.master_key;

      // TODO: can be there other cases?
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool SetAccountPermissions::validate(ametsuchi::WsvQuery &queries,
                                         const Account &creator) {
      // check if creator has permission to set permissions
      return creator.permissions.set_permissions;

      // TODO: can creator set new permissions to their account?
    }

    /**
    *
    * @param queries
    * @param creator
    * @return
    */
    bool SetQuorum::validate(ametsuchi::WsvQuery &queries,
                             const Account &creator) {
      // Case 1: creator sets quorum to their account
      // Quorum must be from 1 to N
      return new_quorum > 0 && new_quorum < 10 &&
             creator.account_id == account_id;

      // TODO: should we consider cases when creator sets quorum to other
    }

    /**
    * Transaction creator can transfer money only from their account.
    * @param queries
    * @param creator
    * @return
    */
    bool TransferAsset::validate(ametsuchi::WsvQuery &queries,
                                 const Account &creator) {
      // TODO: We will get rid off ugly string to float conversion once we have
      // Decimal abstraction
      try {
        // get source account's balance and check if it is sufficient
        auto account_asset = queries.getAccountAsset(src_account_id, asset_id);
        if (std::stod(account_asset.balance) < std::stod(amount)) return false;
      }
      // Check if asset is formed right
      catch (const std::invalid_argument &ia) {
        return false;
      }
      // Check if asset is in range
      catch (const std::out_of_range &oor) {
        return false;
      }

      return
          // Can account transfer assets
          creator.permissions.can_transfer &&
          // Creator can transfer only from their account
          creator.account_id == src_account_id;
    }

  }  // namespace model

}  // namespace iroha
