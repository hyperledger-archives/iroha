/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include <gtest/gtest.h>
#include <random>

#include <util/convert_string.hpp>

TEST(stringify_vector_test, stringify_parse) {
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());

  for (int loop = 0; loop < 100; loop++) {
    std::vector<std::string> vs;

    for (int i=0; i<static_cast<int>(engine()) % 100 + 1; i++) {
      std::string buf;
      for (int k=0; k<static_cast<int>(engine()) % 20; k++) {
        auto which = engine() % 3;
        if (which == 0) {
          buf += '0' + engine() % 10;
        } else if (which == 1) {
          buf += '[';
        } else {
          buf += ']';
        }
      }
      vs.push_back(buf);
    }

    auto str1 = convert_string::stringifyVector(vs);
    auto vec1 = convert_string::parseVector(str1);
    auto str2 = convert_string::stringifyVector(vec1);
    auto vec2 = convert_string::parseVector(str2);

    ASSERT_STREQ(str1.c_str(), str2.c_str());
    ASSERT_TRUE(vec1 == vec2);
  }
}