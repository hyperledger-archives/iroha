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

#ifndef IROHA_COMMAND_VALIDATOR_HPP
#define IROHA_COMMAND_VALIDATOR_HPP

#include <dao/dao>

namespace iroha {
  namespace validation {

    /**
     * Interface for checking invariant after performing command
     */
    class CommandValidator {

      /**
       * Method provide validation of wvs after application command
       * @param command for application
       * @return true if invariant correct, otherwice false
       */
      virtual bool validate(dao::Command &command) = 0;
    };
  } // namespace validation
} // namespace iroha

#endif //IROHA_COMMAND_VALIDATOR_HPP
