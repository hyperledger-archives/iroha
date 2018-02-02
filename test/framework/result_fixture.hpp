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

#ifndef IROHA_RESULT_FIXTURE_HPP
#define IROHA_RESULT_FIXTURE_HPP

#include "common/result.hpp"

namespace framework {
  namespace expected {
    /**
     * Returns value from Result or throws exception if result contains
     * error
     */
    template <typename ResultType>
    typename ResultType::ValueType checkValueCase(const ResultType &result) {
      return boost::get<typename ResultType::ValueType>(result);
    }

    /**
     * Returns error from result or throws exception in case result contains
     * value
     */
    template <typename ResultType>
    typename ResultType::ErrorType checkErrorCase(const ResultType &result) {
      return boost::get<typename ResultType::ErrorType>(result);
    }
  }  // namespace expected
}  // namespace framework

#endif  // IROHA_RESULT_FIXTURE_HPP
