/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
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

#ifndef IROHA_ABSTRACT_VALIDATOR_HPP
#define IROHA_ABSTRACT_VALIDATOR_HPP

#include "validators/answer.hpp"

namespace shared_model {
  namespace validation {

    /**
     * Abstract class for validators
     * @tparam T -- validating type object
     */
    template <typename T>
    class Validator {
     public:
      /**
       * Validates object
       * @param object -- validating object
       * @return answer containing errors
       */
      virtual Answer validate(T object) const = 0;
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_ABSTRACT_VALIDATOR_HPP
