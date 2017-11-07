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

#include "utils/lazy.hpp"
#include <gtest/gtest.h>
#include <string>
#include "logger/logger.hpp"

struct SourceValue {
  int val;
};

struct TargetValue {
  std::string target;
};

/**
 * @given Initialized value
 * @when Call get once
 * @then Assert that transform invoked
 */
TEST(LazyTest, GetterTest) {
  SourceValue v;
  v.val = 100500;
  auto lazy = shared_model::detail::makeLazy(v, [](auto source) {
    TargetValue target_value;
    target_value.target = std::to_string(source.val);
    return target_value;
  });
  ASSERT_EQ("100500", lazy.get().target);
}

/**
 * @given Initialized value
 * @when Call get twice
 * @then Assert that transform invoked once
 */
TEST(LazyTest, CheckLaziness) {
  SourceValue v;
  v.val = 100500;
  auto call_counter = 0;
  auto lazy = shared_model::detail::makeLazy(v, [&call_counter](auto source) {
    call_counter++;
    TargetValue target_value;
    target_value.target = std::to_string(source.val);
    return target_value;
  });
  ASSERT_EQ(0, call_counter);
  ASSERT_EQ("100500", lazy.get().target);
  ASSERT_EQ(1, call_counter);
  ASSERT_EQ("100500", lazy.get().target);
  ASSERT_EQ(1, call_counter);
}
