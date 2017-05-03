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

#include <infra/ametsuchi/include/ametsuchi/ametsuchi.h>
#include <transaction_generated.h>
#include <flatbuffers/flatbuffers.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <iostream>

class Ametsuchi_Test : public ::testing::Test {
protected:
  virtual void TearDown() { system(("rm -rf " + folder).c_str()); }

  std::string folder = "/tmp/ametsuchi/";
  ametsuchi::Ametsuchi ametsuchi_;

  Ametsuchi_Test() : ametsuchi_(folder) {}
};

TEST_F(Iroha_AmetsuchiTest, Asset) {
  std::cout << "HELLO, AMETSUCHI WORLD!!!\n";
  sleep(1);
  std::cout << "THANK YOU\n";
}