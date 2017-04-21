/*
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *          http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_UTILS_EXCEPTION_HPP_
#define IROHA_UTILS_EXCEPTION_HPP_

#include "exception.hpp"

class UnexpectedType {
 public:
  explicit UnexpectedType(exception::IrohaException&& exc) : exc_(exc);
  const exception::IrohaException& exception() const noexcept;

 private:
  exception::IrohaException exc_;
};

UnexpectedType makeUnexpected(exception::IrohaException&& exc);

template <typename T>
class Expected {
 public:
  Expected(
      T&& value,
      typename std::enable_if<!std::is_same<T, UnexpectedType>::value>::type* =
          0)
      : value_(std::forward<T>(value)),
        exc_(exception::IrohaException("")),
        valid_(true) {}

  Expected(UnexpectedType&& exc)
      : value_(T()), exc_(exc.exception()), valid_(false) {}

  constexpr exception::IrohaException& error() noexcept;
  const char* message() const;
};

#endif  // IROHA_UTILS_EXCEPTION_HPP_
