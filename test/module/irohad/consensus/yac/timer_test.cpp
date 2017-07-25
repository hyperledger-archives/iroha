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

#include <gtest/gtest.h>
#include "consensus/yac/impl/timer_impl.hpp"

using namespace iroha::consensus::yac;

TEST(TimerTest, NothingInvokedWhenDenied) {
  int status = 0;
  TimerImpl timer;
  timer.invokeAfterDelay(3 * 1000, [&status]() { status = 1; });
  std::this_thread::sleep_for(std::chrono::seconds(1));
  timer.deny();
  std::this_thread::sleep_for(std::chrono::seconds(3));
  ASSERT_EQ(status, 0);
}

TEST(TimerTest, FirstInvokedWhenOneSubmitted) {
  int status = 0;
  TimerImpl timer;
  timer.invokeAfterDelay(3 * 1000, [&status]() { status = 1; });
  std::this_thread::sleep_for(std::chrono::seconds(4));
  ASSERT_EQ(status, 1);
}

TEST(TimerTest, SecondInvokedWhenTwoSubmitted) {
  int status = 0;
  TimerImpl timer;
  timer.invokeAfterDelay(3 * 1000, [&status]() { status = 1; });
  std::this_thread::sleep_for(std::chrono::seconds(1));
  timer.invokeAfterDelay(3 * 1000, [&status]() { status = 2; });
  std::this_thread::sleep_for(std::chrono::seconds(4));
  ASSERT_EQ(status, 2);
}