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
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit AppendRole(const iroha::protocol::Command &command)
          : AppendRole(command.append_role()) {
        if (not command.has_append_role()) {
          // TODO 11/11/17 andrei create generic exception message
          throw std::invalid_argument(
              "Object does not contain add_asset_quantity");
        }
      }

      const interface::types::AccountIdType & accountId() const {
        return append_role_.account_id();
      }

      const interface::types::RoleIdType & roleName() const {
        return append_role_.role_name();
      }

      const HashType& hash() const {
        return hash_.get();
      }

      ModelType* copy() const {
        return new AppendRole(append_role_);
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit AppendRole(
          const iroha::protocol::AppendRole &append_role)
          : append_role_(append_role),
            hash_([this] {
              // TODO 10/11/2017 muratovv replace with effective implementation
              return crypto::StubHash();
            }) {}

      iroha::protocol::AppendRole append_role_;
      Lazy<crypto::Hash> hash_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_APPEND_ROLE_HPP
