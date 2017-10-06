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
#ifndef IROHA_CREATE_ROLE_HPP
#define IROHA_CREATE_ROLE_HPP

#include <string>
#include "model/command.hpp"

namespace iroha {
  namespace model {

    /**
     * Create new role in the system
     */
    struct CreateRole : public Command {
      /**
       * Role to insert to the system
       */
      std::string role_name{};

      /**
       * Role permissions
       */
      std::vector<std::string> permissions{};

      bool operator==(const Command &command) const override;

      CreateRole() {}

      CreateRole(const std::string &role_name_,
                 const std::vector<std::string> &perms)
          : role_name(role_name_), permissions(perms) {}
    };
  }  // namespace model
}  // namespace iroha
#endif  // IROHA_CREATE_ROLE_HPP
