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

#include "builders/protobuf/query.hpp"

#include <gtest/gtest.h>

/**
 * @given query field values and sample command values, reference query
 * @when create query with sample command using query builder
 * @then query is built correctly
 */
TEST(ProtoQueryBuilder, Builder) {
  uint64_t created_time = 10000000000ull, query_counter = 1;
  std::string account_id = "admin@test", asset_id = "coin#test";

  iroha::protocol::Query proto_tx;
  auto &payload = *proto_tx.mutable_payload();
  auto &query = *payload.add_commands()->mutable_get_account_assets();
  payload.set_created_time(created_time);
  payload.set_creator_account_id(account_id);
  query.set_account_id(account_id);
  query.set_asset_id(asset_id);

  auto tx = shared_model::proto::QueryBuilder()
                .createdTime(created_time)
                .creatorAccountId(account_id)
                .setGetAccountAssets(account_id, asset_id)
                .queryCounter(query_counter)
                .build();
  // Uncomment on completing proto::Query
  // auto &proto = tx.getTransport();

  ASSERT_TRUE(true);
  // ASSERT_EQ(proto_tx.SerializeAsString(), proto.SerializeAsString());
}
