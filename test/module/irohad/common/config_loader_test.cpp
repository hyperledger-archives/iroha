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
#include <common/config.hpp>

#include <cstdlib>
#include <fstream>
class ConfigLoaderTest : public testing::Test {
 public:
  virtual void SetUp() {
    std::ofstream ofs("sample.conf");
    ofs << "{\"name\":\"maruuchi\",\"age\":20, \"isRamen\": false}";
    ofs.close();
  }

  virtual void TearDown() { system("rm sample.conf"); }
};

TEST_F(ConfigLoaderTest, Normal) {
  auto loader = common::config::ConfigLoader("sample.conf");
  ASSERT_STREQ(loader.getStringOrDefault("name", "hibiya").c_str(), "maruuchi");
  ASSERT_EQ(loader.getIntOrDefault("age", 12), 20);
  ASSERT_FALSE(loader.getBoolOrDefault("isRamen", true));
}

TEST_F(ConfigLoaderTest, NoFile) {
  auto loader = common::config::ConfigLoader("sample__.conf");
  ASSERT_STREQ(loader.getStringOrDefault("name", "hibiya").c_str(), "hibiya");
  ASSERT_EQ(loader.getIntOrDefault("age", 12), 12);
  ASSERT_FALSE(loader.getBoolOrDefault("isRamen", false));
}

TEST_F(ConfigLoaderTest, WrongStrKeyName) {
  auto loader = common::config::ConfigLoader("sample.conf");
  ASSERT_STREQ(loader.getStringOrDefault("names", "hibiya").c_str(), "hibiya");
}

TEST_F(ConfigLoaderTest, WrongIntKeyName) {
  auto loader = common::config::ConfigLoader("sample.conf");
  ASSERT_EQ(loader.getIntOrDefault("ago", 12), 12);
}

TEST_F(ConfigLoaderTest, WrongBoolKeyName) {
  auto loader = common::config::ConfigLoader("sample.conf");
  ASSERT_TRUE(loader.getBoolOrDefault("isRamenSanro", true));
}
