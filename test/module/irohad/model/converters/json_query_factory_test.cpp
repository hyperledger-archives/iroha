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
#include "model/generators/query_generator.hpp"

using namespace iroha::model::converters;
using namespace iroha::model::generators;

TEST(QuerySerializerTest, DeserializeGetAccountWhenValid) {
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
  ASSERT_TRUE(res);
  ASSERT_EQ("123",res->creator_account_id);
}

TEST(QuerySerializerTest, DeserializeGetAccountWhenInvalid) {
  JsonQueryFactory querySerializer;
  auto json_query =
      "            {\"created_ts\": 0,\n"
      "            \"creator_account_id\": \"123\",\n"
      "            \"query_counter\": 0,\n"
      "            \"query_type\": \"GetAccount\"\n"
      "                }";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_FALSE(res);
}


TEST(QuerySerializerTest, DeserializeGetAccountAssetsWhenValid) {
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
          "            \"query_type\": \"GetAccountAssets\",\n"
          "            \"account_id\": \"test@test\",\n"
          "            \"asset_id\": \"coin#test\"\n"
          "                }";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_TRUE(res);
  auto casted = std::static_pointer_cast<iroha::model::GetAccountAssets>(res);
  ASSERT_EQ("test@test", casted->account_id);
  ASSERT_EQ("coin#test", casted->asset_id);
}


TEST(QuerySerializerTest, DeserializeWhenUnknownType) {
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
  ASSERT_FALSE(res);
}

TEST(QuerySerializerTest, SerializeGetAccount){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetAccount(0, "123", 0, "test");
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  std::cout << json.value() << std::endl;
}

TEST(QuerySerializerTest, SerializeGetAccountAssets){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetAccountAssets(0, "123", 0, "test", "coin");
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  std::cout << json.value() << std::endl;
}

TEST(QuerySerializerTest, SerializeGetAccountTransactions){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetAccountTransactions(0, "123", 0, "test");
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  std::cout << json.value() << std::endl;
}


TEST(QuerySerializerTest, SerializeGetSignatories){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetSignatories(0, "123", 0, "test");
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  std::cout << json.value() << std::endl;
}
