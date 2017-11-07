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
#include "crypto/hash.hpp"
#include "model/converters/pb_query_factory.hpp"
#include "model/generators/query_generator.hpp"

#include "model/queries/get_roles.hpp"
#include "model/queries/get_asset_info.hpp"

using namespace iroha::model::converters;
using namespace iroha::model::generators;
using namespace iroha::model;

void runQueryTest(std::shared_ptr<Query> query){
  PbQueryFactory queryFactory;
  auto pb_query = queryFactory.serialize(query);
  ASSERT_TRUE(pb_query.has_value());
  auto res_query = queryFactory.deserialize(pb_query.value());
  ASSERT_TRUE(res_query.has_value());
  // TODO 26/09/17 grimadas: overload operator == for queries and replace with it IR-512 #goodfirstissue
  ASSERT_EQ(iroha::hash(*res_query.value()), iroha::hash(*query));
}

TEST(PbQueryFactoryTest, SerializeGetAccount){
  auto createdTime = 111u;
  auto creatorAccountId = "creator";
  auto queryCounter = 222u;
  auto accountId = "test";
  PbQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto query = queryGenerator.generateGetAccount(createdTime, creatorAccountId, queryCounter, accountId);
  auto pb_query = queryFactory.serialize(query);
  ASSERT_TRUE(pb_query.has_value());
  auto &pl = pb_query.value().payload();
  auto &pb_cast = pb_query.value().payload().get_account();
  ASSERT_TRUE(pl.created_time() == createdTime);
  ASSERT_TRUE(pl.creator_account_id() == creatorAccountId);
  ASSERT_TRUE(pl.query_counter() == queryCounter);
  ASSERT_TRUE(pb_cast.account_id() == accountId);
  auto res_query = queryFactory.deserialize(pb_query.value());
  ASSERT_TRUE(res_query.has_value());
  ASSERT_TRUE((*res_query)->created_ts == createdTime);
  ASSERT_TRUE((*res_query)->creator_account_id == creatorAccountId);
  ASSERT_TRUE((*res_query)->query_counter == queryCounter);
  ASSERT_TRUE(((iroha::model::GetAccount&)(**res_query)).account_id == accountId);
  // TODO 26/09/17 grimadas: overload operator == for queries and replace with it IR-512 #goodfirstissue
  ASSERT_EQ(iroha::hash(*res_query.value()), iroha::hash(*query));
}

TEST(PbQueryFactoryTest, SerializeGetAccountAssets){
  PbQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto query = queryGenerator.generateGetAccountAssets(0, "123", 0, "test", "coin");
  auto pb_query = queryFactory.serialize(query);
  ASSERT_TRUE(pb_query.has_value());
  auto res_query = queryFactory.deserialize(pb_query.value());
  ASSERT_TRUE(res_query.has_value());
  // TODO 26/09/17 grimadas: overload operator == for queries and replace with it IR-512 #goodfirstissue
  ASSERT_EQ(iroha::hash(*res_query.value()), iroha::hash(*query));
}

TEST(PbQueryFactoryTest, SerializeGetAccountTransactions){
  PbQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto query = queryGenerator.generateGetAccountTransactions(0, "123", 0, "test");
  auto pb_query = queryFactory.serialize(query);
  ASSERT_TRUE(pb_query.has_value());
  auto res_query = queryFactory.deserialize(pb_query.value());
  ASSERT_TRUE(res_query.has_value());
  // TODO 26/09/17 grimadas: overload operator == for queries and replace with it IR-512 #goodfirstissue
  ASSERT_EQ(iroha::hash(*res_query.value()), iroha::hash(*query));
}

TEST(PbQueryFactoryTest, SerializeGetSignatories){
  PbQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto query = queryGenerator.generateGetSignatories(0, "123", 0, "test");
  auto pb_query = queryFactory.serialize(query);
  ASSERT_TRUE(pb_query.has_value());
  auto res_query = queryFactory.deserialize(pb_query.value());
  ASSERT_TRUE(res_query.has_value());
  // TODO 26/09/17 grimadas: overload operator == for queries and replace with it IR-512 #goodfirstissue
  ASSERT_EQ(iroha::hash(*res_query.value()), iroha::hash(*query));
}

TEST(PbQueryFactoryTest, get_roles){

  auto query = QueryGenerator{}.generateGetRoles();
  runQueryTest(query);
}

TEST(PbQueryFactoryTest, get_role_permissions){
  auto query = QueryGenerator{}.generateGetRolePermissions();
  runQueryTest(query);
}

TEST(PbQueryFactoryTest, get_asset_info){
  auto query = QueryGenerator{}.generateGetAssetInfo();
  runQueryTest(query);
}
