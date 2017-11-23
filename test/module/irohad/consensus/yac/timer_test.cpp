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
#include <thread>
#include "consensus/yac/impl/timer_impl.hpp"

using namespace iroha::consensus::yac;

class TimerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    timer = std::make_shared<TimerImpl>();
  }

  void TearDown() override {
    timer.reset();
  }

 public:
  std::shared_ptr<Timer> timer;
};

TEST_F(TimerTest, NothingInvokedWhenDenied) {
  int status = 0;

  timer->invokeAfterDelay(50, [&status]() { status = 1; });
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  timer->deny();
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  ASSERT_EQ(status, 0);
}

TEST_F(TimerTest, FirstInvokedWhenOneSubmitted) {
  int status = 0;

  timer->invokeAfterDelay(10, [&status]() { status = 1; });
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  ASSERT_EQ(status, 1);
}

TEST_F(TimerTest, SecondInvokedWhenTwoSubmitted) {
  int status = 0;

  timer->invokeAfterDelay(10, [&status]() { status = 1; });
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  timer->invokeAfterDelay(10, [&status]() { status = 2; });
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  ASSERT_EQ(status, 2);
}
