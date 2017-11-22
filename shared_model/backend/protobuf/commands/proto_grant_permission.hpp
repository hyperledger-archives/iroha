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

#ifndef IROHA_PROTO_GRANT_PERMISSION_HPP
#define IROHA_PROTO_GRANT_PERMISSION_HPP

#include "interfaces/commands/grant_permission.hpp"

namespace shared_model {
  namespace proto {

    class GrantPermission final : public interface::GrantPermission {
     private:
      using RefGrantPermission =
          detail::ReferenceHolder<iroha::protocol::Command,
                                  const iroha::protocol::GrantPermission &>;

     public:
      explicit GrantPermission(const iroha::protocol::Command &command)
          : GrantPermission(RefGrantPermission(
                command,
                detail::makeReferenceGetter(
                    &iroha::protocol::Command::grant_permission))) {}

      explicit GrantPermission(iroha::protocol::Command &&command)
          : GrantPermission(RefGrantPermission(
                std::move(command),
                detail::makeReferenceGetter(
                    &iroha::protocol::Command::grant_permission))) {}

      const interface::types::AccountIdType &accountId() const override {
        return grant_permission_->account_id();
      }

      const interface::types::PermissionNameType &permissionName()
          const override {
        return iroha::protocol::GrantablePermission_Name(
            grant_permission_->permission());
      }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_grant_permission() = *grant_permission_;
        return new GrantPermission(std::move(command));
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit GrantPermission(RefGrantPermission &&ref)
          : grant_permission_(std::move(ref)) {}

      RefGrantPermission grant_permission_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_GRANT_PERMISSION_HPP
