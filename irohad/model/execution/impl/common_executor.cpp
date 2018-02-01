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
#include "model/execution/common_executor.hpp"

using namespace iroha::ametsuchi;

namespace iroha {
  namespace model {

    WsvQueryResult<std::set<std::string>> getAccountPermissions(
        const std::string &account_id, iroha::ametsuchi::WsvQuery &queries) {
      auto generate_permissions = [&](const auto &roles) -> WsvQueryResult<std::set<std::string>> {
        std::set<std::string> account_permissions;
        auto append_roles = [&account_permissions](const auto &permissions) {
          account_permissions.insert(permissions.begin(), permissions.end());
        };
        std::for_each(roles.begin(),
                      roles.end(),
                      [&](auto role) {
                        queries.getRolePermissions(role) | append_roles;
                      });
        return expected::makeValue(account_permissions);
      };

      return queries.getAccountRoles(account_id) | generate_permissions;
    }

    bool accountHasPermission(const std::set<std::string> &perms,
                              const std::string &permission_id) {
      return perms.count(permission_id) == 1;
    }

    WsvQueryResult<bool> checkAccountRolePermission(
        const std::string &account_id,
        WsvQuery &queries,
        const std::string &permission_id) {
      auto roleHasPermission = [&permission_id](auto permissions) {
        return std::any_of(
            permissions.begin(),
            permissions.end(),
            [&permission_id](auto perm) { return permission_id == perm; });
      };

      auto checkRolesPermission = [&](auto roles) -> WsvQueryResult<bool> {
        return expected::makeValue(std::any_of(roles.begin(), roles.end(), [&](auto role) {
          return queries.getRolePermissions(role).match(
              [&roleHasPermission](const expected::Value<std::vector<std::string>> &v) {
                return roleHasPermission(v.value);
              },
              [](const expected::Error<WsvError> &e) { return false; }
          );
        }));
      };

      return queries.getAccountRoles(account_id) | checkRolesPermission;
    }

  }  // namespace model
}  // namespace iroha
