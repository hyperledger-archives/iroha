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

#ifndef IROHA_COMMANDS_VALIDATOR_HPP
#define IROHA_COMMANDS_VALIDATOR_HPP

#include <boost/variant/static_visitor.hpp>
#include <regex>
#include "interfaces/common_objects/types.hpp"
#include "interfaces/polymorphic_wrapper.hpp"
#include "interfaces/transaction.hpp"
#include "validators/answer.hpp"

namespace shared_model {
  namespace validation {

    class CommandsValidator {
     public:
      class CommandsValidatorVisitor
          : public boost::static_visitor<ReasonsGroupType> {
       public:
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::AddAssetQuantity> &aaq)
            const {
          ReasonsGroupType reason;
          reason.first = "AddAssetQuantity";

          validateAccountId(reason, aaq->accountId());
          validateAssetId(reason, aaq->assetId());
          validateAmount(reason, aaq->amount());

          return reason;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::AddPeer> &ap) const {
          ReasonsGroupType res;
          res.first = "AddPeer";

          validatePubkey(res, ap->peerKey());
          validatePeerAddress(res, ap->peerAddress());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::AddSignatory> &as)
            const {
          std::string class_name = "AddSignatory";
          ReasonsGroupType res;

          validateAccountId(res, as->accountId());
          validatePubkey(res, as->pubkey());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::AppendRole> &ar) const {
          std::string class_name = "AppendRole";
          ReasonsGroupType res;

          validateAccountId(res, ar->accountId());
          validateRoleId(res, ar->roleName());
          return res;
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::CreateAccount> &ca)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::CreateAsset> &ca)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::CreateDomain> &cd)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::CreateRole> &cr) const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::GrantPermission> &gp)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::RemoveSignatory> &rs)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::RevokePermission> &rp)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::SetQuorum> &sq) const {
          // TODO kamilsa 5.12.17 implement
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::TransferAsset> &ta)
            const {
          // TODO kamilsa 5.12.17 implement
        }

       private:
        void validateAccountId(
            ReasonsGroupType &reason,
            const interface::types::AccountIdType &account_id) const {
          std::regex e("[a-z]{1,9}\\@[a-z]{1,9}");
          if (not std::regex_match(account_id, e)) {
            reason.second.push_back("Wrongly formed account_id");
          }
        }

        void validateAssetId(
            ReasonsGroupType &reason,
            const interface::types::AssetIdType &asset_id) const {
          std::regex e("[a-z]{1,9}\\@[a-z]{1,9}");
          if (not std::regex_match(asset_id, e)) {
            reason.second.push_back("Wrongly formed asset_id");
          }
        }

        void validateAmount(ReasonsGroupType &reason,
                            const interface::Amount &amount) const {
          // what kind of validation could be here?
        }

        void validatePubkey(ReasonsGroupType &reason,
                            const interface::types::PubkeyType &pubkey) const {
          // what constraints can be here?
        }

        void validatePeerAddress(
            ReasonsGroupType &reason,
            const interface::AddPeer::AddressType &address) const {
          // what constraints can be here?
        }

        void validateRoleId(ReasonsGroupType &reason,
                            const interface::types::RoleIdType &role_id) const {
          // what constraints can be here?
        }
      };

      Answer validate(detail::PolymorphicWrapper<interface::Transaction> tx) {
        Answer answer;
        for (auto &command : tx->commands()) {
          answer.addReason(
              boost::apply_visitor(CommandsValidatorVisitor(), command->get()));
        }
        return answer;
      }

     private:
      Answer answer_;
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_COMMANDS_VALIDATOR_HPP
