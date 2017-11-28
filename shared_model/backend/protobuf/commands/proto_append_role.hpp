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

#ifndef IROHA_PROTO_APPEND_ROLE_HPP
#define IROHA_PROTO_APPEND_ROLE_HPP

#include "interfaces/commands/append_role.hpp"

namespace shared_model {
  namespace proto {

    class AppendRole final : public interface::AppendRole {
     public:
      template <typename CommandType>
      explicit AppendRole(CommandType &&command)
          : command_(std::forward<CommandType>(command)),
            append_role_([this] { return command_->append_role(); }) {}

      AppendRole(const AppendRole &o) : AppendRole(*o.command_) {}

      AppendRole(AppendRole &&o) noexcept
          : AppendRole(std::move(o.command_.variant())) {}

      const interface::types::AccountIdType &accountId() const override {
        return append_role_->account_id();
      }

      const interface::types::RoleIdType &roleName() const override {
        return append_role_->role_name();
      }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_append_role() = *append_role_;
        return new AppendRole(std::move(command));
      }

     private:
      // proto
      detail::ReferenceHolder<iroha::protocol::Command> command_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;
      const Lazy<const iroha::protocol::AppendRole &> append_role_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_APPEND_ROLE_HPP
