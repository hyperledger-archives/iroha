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

#include "interfaces/commands/remove_signatory.hpp"

#ifndef IROHA_PROTO_REMOVE_SIGNATORY_HPP
#define IROHA_PROTO_REMOVE_SIGNATORY_HPP

namespace shared_model {
  namespace proto {

    class RemoveSignatory final : public interface::RemoveSignatory {
     private:
      using RefRemoveSignatory =
          detail::ReferenceHolder<iroha::protocol::Command,
                                  const iroha::protocol::RemoveSignatory &>;

     public:
      explicit RemoveSignatory(const iroha::protocol::Command &command)
          : RemoveSignatory(RefRemoveSignatory(
                command,
                detail::makeReferenceGetter(
                    &iroha::protocol::Command::remove_sign))) {}

      explicit RemoveSignatory(iroha::protocol::Command &&command)
          : RemoveSignatory(RefRemoveSignatory(
                std::move(command),
                detail::makeReferenceGetter(
                    &iroha::protocol::Command::remove_sign))) {}

      const interface::types::AccountIdType &accountId() const override {
        return remove_signatory_->account_id();
      }

      const interface::types::PubkeyType &pubkey() const override {
        return *pubkey_;
      }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_remove_sign() = *remove_signatory_;
        return new RemoveSignatory(std::move(command));
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit RemoveSignatory(RefRemoveSignatory &&ref)
          : remove_signatory_(std::move(ref)), pubkey_([this] {
              return interface::types::PubkeyType(
                  remove_signatory_->public_key());
            }) {}

      RefRemoveSignatory remove_signatory_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      Lazy<interface::types::PubkeyType> pubkey_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_REMOVE_SIGNATORY_HPP
