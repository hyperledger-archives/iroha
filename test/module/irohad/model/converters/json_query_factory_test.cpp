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

#include "model/converters/json_query_factory.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace iroha::model::converters;

TEST(QuerySerializerTest, SerializeGetAccountWhenValid) {
  JsonQueryFactory querySerializer;

  auto json_query =
      "{\"signature\": {\n"
      "                    \"pubkey\": "
      "\"2323232323232323232323232323232323232323232323232323232323232323\",\n"
      "                    \"signature\": "
      "\"2323232323232323232323232323232323232323232323232323232323232323232323"
      "2323232323232323232323232323232323232323232323232323232323\"\n"
      "                }, \n"
      "            \"created_ts\": 0,\n"
      "            \"creator_account_id\": \"123\",\n"
      "            \"query_counter\": 0,\n"
      "            \"query_type\": \"GetAccount\",\n"
      "            \"account_id\": \"test@test\"\n"
      "                }";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_TRUE(res.has_value());
  ASSERT_TRUE(res.value().has_get_account());
  ASSERT_EQ(res.value().get_account().account_id(), "test@test");
}

TEST(QuerySerializerTest, SerializeGetAccountWhenInvalid) {
  JsonQueryFactory querySerializer;
  auto json_query =
      "            {\"created_ts\": 0,\n"
      "            \"creator_account_id\": \"123\",\n"
      "            \"query_counter\": 0,\n"
      "            \"query_type\": \"GetAccount\"\n"
      "                }";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_FALSE(res.has_value());
}


TEST(QuerySerializerTest, SerializeGetAccountAssetsWhenValid) {
  JsonQueryFactory querySerializer;
  auto json_query =
      "{\"signature\": {\n"
          "                    \"pubkey\": "
          "\"2323232323232323232323232323232323232323232323232323232323232323\",\n"
          "                    \"signature\": "
          "\"2323232323232323232323232323232323232323232323232323232323232323232323"
          "2323232323232323232323232323232323232323232323232323232323\"\n"
          "                }, \n"
          "            \"created_ts\": 0,\n"
          "            \"creator_account_id\": \"123\",\n"
          "            \"query_counter\": 0,\n"
          "            \"query_type\": \"GetAccountAsset\",\n"
          "            \"account_id\": \"test@test\",\n"
          "            \"asset_id\": \"coin#test\"\n"
          "                }";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_TRUE(res.has_value());
  ASSERT_TRUE(res.value().has_get_account_assets());
  ASSERT_EQ(res.value().get_account_assets().account_id(), "test@test");
  ASSERT_EQ(res.value().get_account_assets().asset_id(), "coin#test");
}


TEST(QuerySerializerTest, SerializeWhenUnknownType) {
  JsonQueryFactory querySerializer;
  auto json_query =
      "{\"signature\": {\n"
          "                    \"pubkey\": "
          "\"2323232323232323232323232323232323232323232323232323232323232323\",\n"
          "                    \"signature\": "
          "\"2323232323232323232323232323232323232323232323232323232323232323232323"
          "2323232323232323232323232323232323232323232323232323232323\"\n"
          "                }, \n"
          "            \"created_ts\": 0,\n"
          "            \"creator_account_id\": \"123\",\n"
          "            \"query_counter\": 0,\n"
          "            \"query_type\": \"GetSomething\",\n"
          "            \"account_id\": \"test@test\",\n"
          "            \"asset_id\": \"coin#test\"\n"
          "                }";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_FALSE(res.has_value());
}
