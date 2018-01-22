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
#include "common/result.hpp"

using iroha::expected::Error;
using iroha::expected::Result;
using iroha::expected::Value;
using iroha::expected::makeError;
using iroha::expected::makeValue;

/**
 * @given Result constructed with valid value
 * @when match function is invoked
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
 * @when match function is invoked
 * @then match executes error case
 */
TEST(ResultTest, ResultErrorConstruction) {
  Result<int, std::string> result = makeError("error message");
  result.match([](Value<int> value) { FAIL() << "Unexpected success case"; },
               [](Error<std::string> error) { SUCCEED(); });
}

/**
 * @given Result constructed with the same type of value and error
 * @when match function is invoked
 * @then match executes successful case
 */
TEST(ResultTest, ResultSameTypeValueConstruction) {
  Result<int, int> result = makeValue(5);
  result.match([](Value<int> value) { SUCCEED(); },
               [](Error<int> error) { FAIL() << "Unexpected error case"; });
}

/**
 * @given Result constructed with the same type of value and error
 * @when match function is invoked
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


// function which must not be called for test to pass
auto never_used_negate_int = [](int a) -> Result<int, std::string> {
  EXPECT_TRUE(false);
  return makeValue(-1 * a);
};

auto error_get_int = []() -> Result<int, std::string> {
  return makeError("first function");
};

/**
 * @given two functions which return result
 * @when bind operator is used to chain these 2 functions and first one return
 *       error
 * @then result of bind contains error and second function is not invoked
 */
TEST(ResultTest, ResultBindOperatorErrorFirstFunction) {
  auto result = error_get_int() | never_used_negate_int;

  result.match([](Value<int> v) { FAIL(); },
               [](Error<std::string> e) {
                 ASSERT_EQ("first function", e.error);
               });
}

/**
 * @given two functions which return different results, but value and error
 *        types can be cast to each other
 * @when bind operator is used to chain these 2 functions
 * @then result type is properly deduced
 */
TEST(ResultTest, ResultBindOperatorCompatibleTypes) {
  auto result = error_get_int() | never_used_negate_int;

  static_assert(std::is_same<decltype(result), decltype(never_used_negate_int(1))>::value,
                "Result type does not match function return type");

  result.match([](Value<int> v) { FAIL(); },
               [](Error<std::string> e) {
                 ASSERT_EQ("first function", e.error);
               });
}
