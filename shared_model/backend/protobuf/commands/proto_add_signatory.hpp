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

#ifndef IROHA_PROTO_ADD_SIGNATORY_HPP
#define IROHA_PROTO_ADD_SIGNATORY_HPP

#include "interfaces/commands/add_signatory.hpp"

namespace shared_model {
  namespace proto {
    class AddSignatory final : public interface::AddSignatory {
     public:
      template <typename CommandType>
      explicit AddSignatory(CommandType &&command)
          : command_(std::forward<CommandType>(command)),
            add_signatory_([this] { return command_->add_signatory(); }),
            pubkey_([this] {
              return interface::types::PubkeyType(add_signatory_->public_key());
            }) {}

      AddSignatory(const AddSignatory &o) : AddSignatory(*o.command_) {}

      AddSignatory(AddSignatory &&o) noexcept
          : AddSignatory(std::move(o.command_.variant())) {}

      const interface::types::AccountIdType &accountId() const override {
        return add_signatory_->account_id();
      }

      const interface::types::PubkeyType &pubkey() const override {
        return *pubkey_;
      }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_add_signatory() = *add_signatory_;
        return new AddSignatory(std::move(command));
      }

     private:
      // proto
      detail::ReferenceHolder<iroha::protocol::Command> command_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;
      const Lazy<const iroha::protocol::AddSignatory &> add_signatory_;
      Lazy<interface::types::PubkeyType> pubkey_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ADD_SIGNATORY_HPP
