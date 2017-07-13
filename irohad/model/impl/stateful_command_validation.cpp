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
      // Check if creator has MoneyCreator permission
      return creator.permissions.issue_assets &&
             // Asset must exist in the system
             !queries.getAsset(asset_id).name.empty();
      // Check if the amount if meaningful
      // TODO: check if amount is in some scope
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


  }  // namespace model

}  // namespace iroha
