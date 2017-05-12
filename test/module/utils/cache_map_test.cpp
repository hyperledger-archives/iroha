/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
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

//
// Created by Takumi Yamashita on 2017/04/28.
//

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <utils/cache_map.hpp>

TEST(CacheMapTest, CacheMapTest) {
  const int N = 10;
  structure::CacheMap<int, std::string> cmap(N);

  int is[] = {100, 321, 2, 4, 31, 32, 55, 66, 44, 88};
  std::string vs[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};

  ASSERT_TRUE(cmap.max_size() == N);

  // add and getMaxKey Test
  int maxi = 0;
  for (int i = 0; i < N; i++) {
    maxi = std::max(maxi, is[i]);
    cmap.set(is[i], vs[i]);
    ASSERT_TRUE(cmap.getMaxKey() == maxi);
  }

  // check key = value Test
  for (int i = 0; i < N; i++) {
    ASSERT_TRUE(cmap[is[i]] == vs[i]);
  }
  // change size Test
  ASSERT_TRUE(cmap.size() == N);
  cmap.set_cache_size(5);
  ASSERT_TRUE(cmap.size() == 5);

  ASSERT_TRUE(cmap.getMaxKey() == 88);
  for (int i = 0; i < 5; i++) {
    bool except_flag = false;
    try {
      cmap[is[i]];
    } catch (std::out_of_range& e) {
      except_flag = true;
    }
    ASSERT_TRUE(except_flag);
  }
  for (int i = 5; i < N; i++) {
    ASSERT_TRUE(cmap[is[i]] == vs[i]);
  }
}
