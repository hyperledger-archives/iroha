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

#include <model/commands/add_asset_quantity.hpp>
#include <model/commands/add_signatory.hpp>

using namespace iroha::model;

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
         // Check if account exist
         !queries.getAccount(account_id).user_name.empty() &&
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
      // Case 1. When command creator wants to add his/her signatory to some
      // account
      creator.master_key == pubkey;
  // TODO: What about other cases ?
}
