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

#include "backend/protobuf/transaction.hpp"
#include "converters/protobuf/json_proto_converter.hpp"

using namespace shared_model::converters::protobuf;

/**
 * creates sample transaction
 * @return tx shared model object
 */
shared_model::proto::Transaction generateTransaction() {
  shared_model::interface::types::CounterType tx_counter = 1;
  std::string creator_account_id = "admin@test";

  iroha::protocol::Transaction proto_tx;
  auto &payload = *proto_tx.mutable_payload();
  payload.set_tx_counter(tx_counter);
  payload.set_creator_account_id(creator_account_id);
  payload.set_created_time(123);

  return shared_model::proto::Transaction(
      iroha::protocol::Transaction(proto_tx));
}

/**
 * creates sample block containing several txs
 * @return block shared model object
 */
shared_model::proto::Block generateBlock() {
  size_t tx_number = 10;
  iroha::protocol::Block block;
  for (size_t i = 0; i < tx_number; i++) {
    auto tx = generateTransaction();
    block.mutable_payload()->add_transactions()->CopyFrom(tx.getTransport());
  }

  return shared_model::proto::Block(iroha::protocol::Block(block));
}

/**
 * @given sample transaction shared model object
 * @when tranasaction is converted to json and then converted back to shared model object
 * @then original and obtained objects are equal
 */
TEST(JsonProtoConverterTest, JsonToProtoTxTest) {
  auto orig_tx = generateTransaction();
  auto json = modelToJson(orig_tx);

  auto obtained_tx = jsonToTransaction(json);
  ASSERT_TRUE(obtained_tx);
  ASSERT_EQ(orig_tx.getTransport().SerializeAsString(),
            obtained_tx.value().getTransport().SerializeAsString());
}

/**
 * @given sample block shared model object
 * @when block is converted to json and then converted back to shared model object
 * @then original and obtained objects are equal
 */
TEST(JsonProtoConverterTest, JsonToProtoBlockTest) {
  auto orig_block = generateBlock();
  auto json = modelToJson(orig_block);

  auto obtained_block = jsonToBlock(json);
  ASSERT_TRUE(obtained_block);
  ASSERT_EQ(orig_block.getTransport().SerializeAsString(),
            obtained_block.value().getTransport().SerializeAsString());
}
