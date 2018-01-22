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

#include <gtest/gtest.h>
#include <common/result.hpp>

using iroha::expected::Error;
using iroha::expected::Result;
using iroha::expected::Value;
using iroha::expected::makeError;
using iroha::expected::makeValue;

/**
 * @given Result constructed with valid value
 * @when when match function is invoked
 * @then match executes successful case
 */
TEST(ResultTest, ResultValueConstruction) {
  Result<int, std::string> result = makeValue(5);
  result.match(
      [](Value<int> value) { SUCCEED(); },
      [](Error<std::string> error) { FAIL() << "Unexpected error case"; });
}

/**
 * @given Result constructed with error value
 * @when when match function is invoked
 * @then match executes error case
 */
TEST(ResultTest, ResultErrorConstruction) {
  Result<int, std::string> result = makeError("error message");
  result.match([](Value<int> value) { FAIL() << "Unexpected success case"; },
               [](Error<std::string> error) { SUCCEED(); });
}

/**
 * @given Result constructed with the same type of value and error
 * @when when match function is invoked
 * @then match executes successful case
 */
TEST(ResultTest, ResultSameTypeValueConstruction) {
  Result<int, int> result = makeValue(5);
  result.match([](Value<int> value) { SUCCEED(); },
               [](Error<int> error) { FAIL() << "Unexpected error case"; });
}

/**
 * @given Result constructed with the same type of value and error
 * @when when match function is invoked
 * @then match executes error case
 */
TEST(ResultTest, ResultSameTypeErrorConstruction) {
  Result<int, int> result = makeError(10);
  result.match([](Value<int> value) { FAIL() << "Unexpected success case"; },
               [](Error<int> error) { SUCCEED(); });
}

/**
 * @given Two functions which return result
 * @when bind operator is used to chain these 2 functions and they both return
 *       valid values
 * @then result of a bind contains valid value.
 */
TEST(ResultTest, ResultBindOperatorSuccesfulCase) {
  auto get_int = []() -> Result<int, std::string> { return makeValue(10); };
  auto negate_int = [](int a) -> Result<int, std::string> {
    return makeValue(-1 * a);
  };

  auto result = get_int() | negate_int;

  result.match([](Value<int> v) { ASSERT_EQ(-10, v.value); },
               [](Error<std::string> e) { FAIL() << "Unexpected error case"; });
}

/**
 * @given Two functions which return result
 * @when bind operator is used to chain these 2 functions and first one return
 *       error
 * @then result of bind contains error and second function is not invoked
 */
TEST(ResultTest, ResultBindOperatorErrorFirstFunction) {
  auto call_counter = 0;
  auto get_int = []() -> Result<int, std::string> {
    return makeError("first function");
  };

  // we cannot use FAIL, or ASSERTs because compiler fails to match return types
  auto negate_int = [&call_counter](int a) -> Result<int, std::string> {
    call_counter++;
    return makeValue(-1 * a);
  };

  auto result = get_int() | negate_int;

  result.match([](Value<int> v) { FAIL(); },
               [&call_counter](Error<std::string> e) {
                 ASSERT_EQ("first function", e.error);
                 ASSERT_EQ(0, call_counter);
               });
}

/**
 * @given Two functions which return different results, but value and error
 *        types can be cast to each other
 * @when bind operator is used to chain these 2 functions
 * @then result type is properly deduced
 */
TEST(ResultTest, ResultBindOperatorCompatibleTypes) {
  auto call_counter = 0;
  auto get_int = []() -> Result<int, const char *> {
    return makeError("first function");
  };

  // we cannot use FAIL, or ASSERTs because compiler fails to match return types
  auto negate_int = [&call_counter](int a) -> Result<int, std::string> {
    call_counter++;
    return makeValue(-1 * a);
  };

  auto result = get_int() | negate_int;

  static_assert(std::is_same<decltype(result), decltype(negate_int(1))>::value,
                "Result type does not match function return type");

  result.match([](Value<int> v) { FAIL(); },
               [&call_counter](Error<std::string> e) {
                 ASSERT_EQ("first function", e.error);
                 ASSERT_EQ(0, call_counter);
               });
}
