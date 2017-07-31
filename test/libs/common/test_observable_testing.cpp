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

#include "common/test_observable.hpp"
#include <gtest/gtest.h>

#include <iostream>

using namespace common::test_observable;

class TestObservableTesting : public ::testing::Test {

};

TEST_F(TestObservableTesting, ValidCallExactTest) {
  auto ints = rxcpp::observable<>::create<int>(
      [](auto s) {
        s.on_next(1);
        s.on_next(2);
        s.on_completed();
      });

  auto number_of_calls = 0;

  auto wrapper = make_test_observable<CallExact>(ints, 2);
  wrapper.subscribe([&number_of_calls](auto val) { ++number_of_calls; });

  ASSERT_TRUE(wrapper.validate());
  ASSERT_EQ(2, number_of_calls);
}

TEST_F(TestObservableTesting, UnsatisfiedCallExactTest) {
  auto ints = rxcpp::observable<>::create<int>(
      [](auto s) {
        s.on_next(1);
        s.on_next(2);
        s.on_completed();
      });

  auto number_of_calls = 0;

  auto wrapper = make_test_observable<CallExact>(ints, 555);
  wrapper.subscribe([&number_of_calls](auto val) { ++number_of_calls; });
  ASSERT_FALSE(wrapper.validate());
  ASSERT_EQ(2, number_of_calls);
}

TEST_F(TestObservableTesting, DefaultSubscriberTest) {
  auto one = rxcpp::observable<>::just(0);

  auto wrapper = make_test_observable<CallExact>(one, 1);
  wrapper.subscribe();

  ASSERT_TRUE(wrapper.validate());
}