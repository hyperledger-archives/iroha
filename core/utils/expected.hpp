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
  explicit UnexpectedType(exception::IrohaException&& exc) : exc_(exc) {}
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

  Expected(const UnexpectedType& exc)
      : value_(T()), exc_(exc.exception()), valid_(false) {}

  bool valid() const { return valid_; }
  explicit operator bool() const { return valid(); }

  inline T& value() {
    if (!valid()) throw exc_;
    return value_;
  }

  inline T& operator*() { return value(); }

  exception::IrohaException& exception() noexcept { return exc_; }
  const char* error() const { return exc_.message(); }

 private:
  T value_;
  exception::IrohaException exc_;
  bool valid_;
};

class VoidHandler {
 public:
  VoidHandler() : exc_(exception::None()), valid_(true) {}

  template <typename T>
  VoidHandler(
      T&& value,
      typename std::enable_if<!std::is_same<T, UnexpectedType>::value>::type* =
          0)
      : exc_(exception::IrohaException("")), valid_(true) {}

  VoidHandler(const UnexpectedType& exc) : exc_(exc.exception()), valid_(false) {}

  bool valid() const { return valid_; }
  explicit operator bool() const { return valid(); }

  exception::IrohaException& exception() noexcept { return exc_; }
  const char* error() const { return exc_.message(); }

 private:
  exception::IrohaException exc_;
  bool valid_;
};

#endif  // IROHA_UTILS_EXCEPTION_HPP_
