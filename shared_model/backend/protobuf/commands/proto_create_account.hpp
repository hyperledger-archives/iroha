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

#ifndef IROHA_PROTO_CREATE_ACCOUNT_HPP
#define IROHA_PROTO_CREATE_ACCOUNT_HPP

#include "interfaces/commands/create_account.hpp"

namespace shared_model {
  namespace proto {

    class CreateAccount final : public interface::CreateAccount {
     public:
      template <typename CommandType>
      explicit CreateAccount(CommandType &&command)
          : command_(std::forward<CommandType>(command)),
      create_account_([this] { return command_->create_account(); }),
            pubkey_([this] {
              return interface::types::PubkeyType(
                  create_account_->main_pubkey());
            }) {}

      CreateAccount(const CreateAccount &o) : CreateAccount(*o.command_) {}

      CreateAccount(CreateAccount &&o) noexcept
          : CreateAccount(std::move(o.command_.variant())) {}

      const interface::types::PubkeyType &pubkey() const override {
        return *pubkey_;
      }

      const AccountNameType &accountName() const override {
        return create_account_->account_name();
      }

      const interface::types::DomainIdType &domainId() const override {
        return create_account_->domain_id();
      }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_create_account() = *create_account_;
        return new CreateAccount(std::move(command));
      }

     private:
      // proto
      detail::ReferenceHolder<iroha::protocol::Command> command_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      const Lazy<const iroha::protocol::CreateAccount &> create_account_;
      Lazy<interface::types::PubkeyType> pubkey_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_CREATE_ACCOUNT_HPP
