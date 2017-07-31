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

#include <algorithm>
#include <limits>

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
      return
          // Amount must be in some meaningful range
          (amount.int_part > 0 || amount.frac_part > 0) &&
          amount.int_part < std::numeric_limits<uint32_t>::max() &&
          // Check if creator has MoneyCreator permission
          creator.permissions.issue_assets;
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
          // Case 1. When command creator wants to add signatory to their
          // account
          account_id == creator.account_id ||
          // Case 2. System admin wants to add signatory to account
          (creator.permissions.add_signatory);
    }

    bool AddPeer::validate(ametsuchi::WsvQuery &queries,
                           const Account &creator) {
      // TODO: check that address is formed right
      return true;
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool AssignMasterKey::validate(ametsuchi::WsvQuery &queries,
                                   const Account &creator) {
      auto signs = queries.getSignatories(account_id);
      auto acc = queries.getAccount(account_id);
      return
          // Two cases - when creator assigns to itself, or system admin
          (creator.account_id == account_id ||
           creator.permissions.add_signatory) &&
          // Check if account exist and has at least one signatory
          signs && acc.has_value() &&
          // Check if new master key is not the same
          acc.value().master_key != pubkey &&
          // Check if new master key is in AccountSignatory relationship
          std::find(signs->begin(), signs->end(), pubkey) != signs->end();
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
      return creator.permissions.create_accounts &&
             // Name is within some range
             account_name.size() > 0 && account_name.size() < 8 &&
             // Account must be well-formed (no system symbols)
             std::all_of(std::begin(account_name), std::end(account_name),
                         [](char c) { return std::isalnum(c); });
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
      return creator.permissions.create_assets &&
             // Name is within some range
             asset_name.size() > 0 && asset_name.size() < 10 &&
             // Account must be well-formed (no system symbols)
             std::all_of(std::begin(asset_name), std::end(asset_name),
                         [](char c) { return std::isalnum(c); });
      ;
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
      return creator.permissions.create_domains &&
             // Name is within some range
             domain_name.size() > 0 && domain_name.size() < 10 &&
             // Account must be well-formed (no system symbols)
             std::all_of(std::begin(domain_name), std::end(domain_name),
                         [](char c) { return std::isalnum(c); });
      ;
    }

    /**
     *
     * @param queries
     * @param creator
     * @return
     */
    bool RemoveSignatory::validate(ametsuchi::WsvQuery &queries,
                                   const Account &creator) {
      auto account = queries.getAccount(account_id);
      return
          // Two cases possible.
          // 1. Creator removes signatory from their account
          // 2.System admin
          account.has_value() && (creator.account_id == account_id ||
                                  creator.permissions.remove_signatory) &&
          // You can't remove master key (first you should reassign it)
          pubkey != account.value().master_key;
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
    }

    /**
    *
    * @param queries
    * @param creator
    * @return
    */
    bool SetQuorum::validate(ametsuchi::WsvQuery &queries,
                             const Account &creator) {
      // Quorum must be from 1 to N
      return new_quorum > 0 && new_quorum < 10 &&
             // Case 1: creator sets quorum to their account
             // Case 2: system admin
             (creator.account_id == account_id ||
              creator.permissions.set_quorum);
    }

    /**
    * Transaction creator can transfer money only from their account.
    * @param queries
    * @param creator
    * @return
    */
    bool TransferAsset::validate(ametsuchi::WsvQuery &queries,
                                 const Account &creator) {
      auto account_asset = queries.getAccountAsset(src_account_id, asset_id);
      auto asset = queries.getAsset(asset_id);
      if (!account_asset || !asset) return false;
      // Amount is formed wrong
      if (amount.get_frac_number() > asset.value().precision) return false;

      return
          // Check if src_account exist
          queries.getAccount(src_account_id) &&
          // Check if dest account exist
          queries.getAccount(dest_account_id) &&
          // Can account transfer assets
          creator.permissions.can_transfer &&
          // Creator can transfer only from their account
          creator.account_id == src_account_id &&
          // Balance in your wallet should be at least amount of transfer
          account_asset.value().balance >=
              amount.get_joint_amount(asset.value().precision) &&
          // Amount must be not zero
          (amount.frac_part > 0 || amount.int_part > 0);
    }

  }  // namespace model

}  // namespace iroha
