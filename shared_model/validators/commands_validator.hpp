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
      class CommandsValidatorVisitor : public boost::static_visitor<Answer> {
       public:
        Answer operator()(
            const detail::PolymorphicWrapper<interface::AddAssetQuantity> &aaq)
            const {
          std::string class_name = "AddAssetQuantity";
          Answer res;

          if (not validateAccountId(aaq->accountId())) {
            res.addReason(class_name, "Wrongly formed account_id");
          }
          if (not validateAssetId(aaq->assetId())) {
            res.addReason(class_name, "Wrongly formed asset_id");
          }
          if (not validateAmount(aaq->amount())) {
            res.addReason(class_name, "Wrongly formed amount");
          }
          return res;
        }

        Answer operator()(
            const detail::PolymorphicWrapper<interface::AddPeer> &ap) const {
          std::string class_name = "AddPeer";
          Answer res;

          if (not validatePubkey(ap->peerKey())){
            res.addReason(class_name, "Wrongly formed amount");
          }

          if (not validatePeerAddress(ap->peerAddress())){
            res.addReason(class_name, "Wrongly formed peer address");
          }

          return res;

        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::AddSignatory> &as)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::AppendRole> &ar) const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::CreateAccount> &ca)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::CreateAsset> &ca)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::CreateDomain> &cd)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::CreateRole> &cr) const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::GrantPermission> &gp)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::RemoveSignatory> &rs)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::RevokePermission> &rp)
            const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::SetQuorum> &sq) const {
          // TODO kamilsa 5.12.17 implement
        }
        Answer operator()(
            const detail::PolymorphicWrapper<interface::TransferAsset> &ta)
            const {
          // TODO kamilsa 5.12.17 implement
        }

       private:
        bool validateAccountId(
            const interface::types::AccountIdType &account_id) const {
          std::regex e("[a-z]{1,9}\\@[a-z]{1,9}");
          return std::regex_match(account_id, e);
        }

        bool validateAssetId(
            const interface::types::AssetIdType &asset_id) const {
          std::regex e("[a-z]{1,9}\\@[a-z]{1,9}");
          return std::regex_match(asset_id, e);
        }

        bool validateAmount(const interface::Amount &amount) const {
          // what kind of validation could be here?
          return true;
        }

        bool validatePubkey(const interface::types::PubkeyType &pubkey) const {
          // what constraints here?
          return true;
        }

        bool validatePeerAddress(const interface::AddPeer::AddressType &address) const {
          // what constraints here?
          return true;
        }
      };

      Answer validate(detail::PolymorphicWrapper<interface::Transaction> tx) {
        for (auto &command : tx->commands()) {
          boost::apply_visitor(CommandsValidatorVisitor(), command->get());
        }
      }
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_COMMANDS_VALIDATOR_HPP
