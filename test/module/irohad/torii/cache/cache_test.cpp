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

#include "torii/cache/cache.hpp"
#include <endpoint.pb.h>
#include <gtest/gtest.h>

using namespace torii::cache;
using namespace iroha::protocol;

const int typicalInsertAmount = 5;

/**
 * @given initialized cache
 * @when insert N ToriiResponse objects into it
 * @then amount of items in cache equals N
 */
TEST(CacheTest, InsertValues) {
  ToriiResponseCache cache;
  ASSERT_EQ(cache.getCacheItemCount(), 0);
  for (int i = 0; i < typicalInsertAmount; ++i) {
    ToriiResponse response;
    response.set_tx_status(TxStatus::STATELESS_VALIDATION_SUCCESS);
    cache.addItem(response, "abcdefg" + std::to_string(i));
  }
  ASSERT_EQ(cache.getCacheItemCount(), typicalInsertAmount);
}

/**
 * @given initialized cache
 * @when insert cache.getIndexSizeHigh() items into it + 1
 * @then after the last insertion amount of items should decrease to
 * cache.getIndexSizeLow()
 */
TEST(CacheTest, InsertMoreThanLimit) {
  ToriiResponseCache cache;
  for (uint32_t i = 0; i < cache.getIndexSizeHigh(); ++i) {
    ToriiResponse response;
    response.set_tx_status(TxStatus::STATEFUL_VALIDATION_FAILED);
    cache.addItem(response, "abcdefg" + std::to_string(i));
  }
  ASSERT_EQ(cache.getCacheItemCount(), cache.getIndexSizeHigh());
  ToriiResponse resp;
  resp.set_tx_status(TxStatus::COMMITTED);
  cache.addItem(resp, "1234");
  ASSERT_EQ(cache.getCacheItemCount(), cache.getIndexSizeLow());
}

/**
 * @given initialized cache
 * @when insert N items and then insert 2 with the same hashes
 * @then amount of cache items should not increase after last 2 insertions
 * but their statuses should be updated
 */
TEST(CacheTest, InsertSameHashes) {
  ToriiResponseCache cache;
  for (int i = 0; i < typicalInsertAmount; ++i) {
    ToriiResponse response;
    response.set_tx_status(TxStatus::NOT_RECEIVED);
    cache.addItem(response, std::to_string(i));
  }
  ToriiResponse resp;
  resp.set_tx_status(TxStatus::COMMITTED);
  cache.addItem(resp, "0");
  ASSERT_EQ(cache.getCacheItemCount(), typicalInsertAmount);
  ASSERT_EQ(cache.findItem("0")->tx_status(), TxStatus::COMMITTED);
  cache.addItem(resp, "1");
  ASSERT_EQ(cache.getCacheItemCount(), typicalInsertAmount);
  ASSERT_EQ(cache.findItem("1")->tx_status(), TxStatus::COMMITTED);
}

/**
 * @given Initialized cache
 * @when insert N items and find one of them
 * @then item should be found and its status should be the same as before
 * insertion
 */
TEST(CacheTest, FindValues) {
  ToriiResponseCache cache;
  auto item = cache.findItem("0");
  ASSERT_EQ(item, boost::none);
  for (int i = 0; i < typicalInsertAmount; ++i) {
    ToriiResponse response;
    response.set_tx_status(TxStatus::STATEFUL_VALIDATION_SUCCESS);
    cache.addItem(response, std::to_string(i));
  }
  item = cache.findItem("2");
  ASSERT_NE(item, boost::none);
  ASSERT_EQ(item->tx_status(), TxStatus::STATEFUL_VALIDATION_SUCCESS);
}

/**
 * @given Initialized cache
 * @when find something in cache
 * @then item should not be found
 */
TEST(CacheTest, FindInEmptyCache) {
  ToriiResponseCache cache;
  auto item = cache.findItem("0");
  ASSERT_EQ(item, boost::none);
}
