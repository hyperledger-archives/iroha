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
#include "common/types.hpp"

namespace iroha {
  namespace model {

    bool checkAccountRolePermission(const std::string &account_id,
                                    WsvQuery &queries,
                                    const std::string &permission_id) {
      auto roleHasPermission = [permission_id](auto permissions) {
        return std::any_of(
            permissions.begin(), permissions.end(),
            [&permission_id](auto perm) { return permission_id == perm; });
      };

      auto checkRolesPermission = [queries, permission_id](auto roles) {
        return std::any_of(roles.begin(), roles.end(),
                           [&queries, &permission_id](auto role) {
                             return queries.getRolePermissions(role)
                                 | roleHasPermission(permission_id);
                           });
      };

      return queries.getAccountRoles(account_id)
          | checkRolesPermission(queries, permission_id);
    }

  }  // namespace model
}  // namespace iroha