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
          std::string class_name = "CreateAccount";
          ReasonsGroupType res;

          validatePubkey(res, ca->pubkey());
          validateAccountName(res, ca->accountName());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::CreateAsset> &ca)
            const {
          std::string class_name = "CreateAccount";
          ReasonsGroupType res;

          validateAssetName(res, ca->assetName());
          validateDomainId(res, ca->domainId());
          validatePrecision(res, ca->precision());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::CreateDomain> &cd)
            const {
          std::string class_name = "CreateDomain";
          ReasonsGroupType res;

          validateDomainId(res, cd->domainId());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::CreateRole> &cr) const {
          std::string class_name = "CreateRole";
          ReasonsGroupType res;

          validateRoleId(res, cr->roleName());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::GrantPermission> &gp)
            const {
          std::string class_name = "GrantPermission";
          ReasonsGroupType res;

          validateAccountId(res, gp->accountId());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::RemoveSignatory> &rs)
            const {
          std::string class_name = "RemoveSignatory";
          ReasonsGroupType res;

          validateAccountId(res, rs->accountId());
          validatePubkey(res, rs->pubkey());

          return res;
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::RevokePermission> &rp)
            const {
          std::string class_name = "RevokePermission";
          ReasonsGroupType res;

          validateAccountId(res, rp->accountId());
          validatePermission(res, rp->permissionName());

          return res;
        }
        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::SetQuorum> &sq) const {
          std::string class_name = "SetQuorum";
          ReasonsGroupType res;

          validateAccountId(res, sq->accountId());
          validateQuorum(res, sq->newQuorum());

          return res;
        }

        ReasonsGroupType operator()(
            const detail::PolymorphicWrapper<interface::TransferAsset> &ta)
            const {
          std::string class_name = "SetQuorum";
          ReasonsGroupType res;

          validateAccountId(res, ta->srcAccountId());
          validateAccountId(res, ta->destAccountId());
          validateAssetId(res, ta->assetId());
          validateAmount(res, ta->amount());

          return res;
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
          std::regex e("[a-z]{1,9}\\#[a-z]{1,9}");
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

        void validateAccountName(ReasonsGroupType &reason,
                                 const interface::CreateAccount::AccountNameType
                                     &account_name) const {
          std::regex e("[a-z]{1,9}");
          if (not std::regex_match(account_name, e)) {
            reason.second.push_back("Wrongly formed account_name");
          }
        }

        void validateDomainId(
            ReasonsGroupType &reason,
            const interface::types::DomainIdType &domain_id) const {
          std::regex e("[a-z]{1,9}");
          if (not std::regex_match(domain_id, e)) {
            reason.second.push_back("Wrongly formed domain_id");
          }
        }

        void validateAssetName(
            ReasonsGroupType &reason,
            const interface::CreateAsset::AssetNameType &asset_name) const {
          std::regex e("[a-z]{1,9}");
          if (not std::regex_match(asset_name, e)) {
            reason.second.push_back("Wrongly formed asset_name");
          }
        }

        void validatePrecision(
            ReasonsGroupType &reason,
            const interface::types::PrecisionType &precision) const {
          // define precision constraints
        }

        void validatePermission(
            ReasonsGroupType &reason,
            const interface::types::PermissionNameType &permission_name) const {
          // define permission constraints
        }

        void validateQuorum(
            ReasonsGroupType &reason,
            const interface::types::QuorumType &quorum) const {
          // define quorum constraints
        }

      };

      Answer validate(detail::PolymorphicWrapper<interface::Transaction> tx) {
        Answer answer;
        for (auto &command : tx->commands()) {
          auto reason =
              boost::apply_visitor(CommandsValidatorVisitor(), command->get());
          if (not reason.second.empty()) {
            answer.addReason(std::move(reason));
          }
        }
        return answer;
      }

     private:
      Answer answer_;
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_COMMANDS_VALIDATOR_HPP
