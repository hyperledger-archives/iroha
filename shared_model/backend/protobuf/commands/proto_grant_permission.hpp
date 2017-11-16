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
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit GrantPermission(const iroha::protocol::Command &command)
          : GrantPermission(command.grant_permission()) {
        if (not command.has_grant_permission()) {
          // TODO 11/11/17 andrei create generic exception message
          throw std::invalid_argument("Object does not contain create_asset");
        }
      }

      const interface::types::AccountIdType &accountId() const override {
        return grant_permission_.account_id();
      }

      const interface::types::PermissionNameType &permissionName()
          const override {
        return iroha::protocol::GrantablePermission_Name(
            grant_permission_.permission());
      }

      const HashType &hash() const override { return hash_.get(); }

      ModelType *copy() const override {
        return new GrantPermission(grant_permission_);
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit GrantPermission(
          const iroha::protocol::GrantPermission &grant_permission)
          : grant_permission_(grant_permission), hash_([this] {
              // TODO 10/11/2017 muratovv replace with effective implementation
              return crypto::StubHash();
            }) {}

      iroha::protocol::GrantPermission grant_permission_;
      Lazy<crypto::Hash> hash_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_GRANT_PERMISSION_HPP
