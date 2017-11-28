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

#ifndef IROHA_PROTO_CREATE_DOMAIN_HPP
#define IROHA_PROTO_CREATE_DOMAIN_HPP

#include "interfaces/commands/create_domain.hpp"

namespace shared_model {
  namespace proto {

    class CreateDomain final : public interface::CreateDomain {
     public:
      template <typename CommandType>
      explicit CreateDomain(CommandType &&command)
          : command_(std::forward<CommandType>(command)),
            create_domain_([this] { return command_->create_domain(); }) {}

      CreateDomain(const CreateDomain &o) : CreateDomain(*o.command_) {}

      CreateDomain(CreateDomain &&o) noexcept
          : CreateDomain(std::move(o.command_.variant())) {}

      const interface::types::DomainIdType &domainId() const override {
        return create_domain_->domain_id();
      }

      const interface::types::RoleIdType &userDefaultRole() const override {
        return create_domain_->default_role();
      }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_create_domain() = *create_domain_;
        return new CreateDomain(std::move(command));
      }

     private:
      // proto
      detail::ReferenceHolder<iroha::protocol::Command> command_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;
      const Lazy<const iroha::protocol::CreateDomain &> create_domain_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_CREATE_DOMAIN_HPP
