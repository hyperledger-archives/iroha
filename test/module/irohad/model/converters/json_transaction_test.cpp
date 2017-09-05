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
#include "model/converters/json_transaction_factory.hpp"
#include "model/converters/json_common.hpp"
#include "model/commands/add_asset_quantity.hpp"

using namespace iroha;
using namespace iroha::model;
using namespace iroha::model::converters;

class JsonTransactionTest : public ::testing::Test {
 public:
  JsonTransactionFactory factory;
};

TEST_F(JsonTransactionTest, ValidWhenWellFormed){
  Transaction transaction{};
  transaction.signatures.emplace_back();
  transaction.commands.push_back(std::make_shared<AddAssetQuantity>());

  auto json_transaction = factory.serialize(transaction);
  auto serial_transaction = factory.deserialize(json_transaction);

  ASSERT_TRUE(serial_transaction.has_value());
  ASSERT_EQ(transaction, serial_transaction.value());
}

TEST_F(JsonTransactionTest, InvalidWhenFieldsMissing){
  Transaction transaction{};

  auto json_transaction = factory.serialize(transaction);
  
  json_transaction.RemoveMember("created_ts");
  
  auto serial_transaction = factory.deserialize(json_transaction);

  ASSERT_FALSE(serial_transaction.has_value());
}

TEST_F(JsonTransactionTest, InvalidWhenNegativeAddAssetQuantity) {
  auto transaction_string = "{\n"
      "    \"signatures\": [\n"
      "        {\n"
      "            \"pubkey\": \"f24325aa9b91526a83e722e19fa2a3ad7f3966abe066a9302b5d2092fe960254\",\n"
      "            \"signature\": \"4563f8de6ee45e44b462a7027d1640376dece03eaf1091f8e69cdc9531957b178f00667e9241ba2d09f49b7861419b89af4986eb4d332e9d7efb31bb1105890e\"\n"
      "        }\n"
      "    ],\n"
      "    \"created_ts\": 1503845603221,\n"
      "    \"creator_account_id\": \"admin@test\",\n"
      "    \"tx_counter\": 1,\n"
      "    \"commands\": [\n"
      "        {\n"
      "            \"command_type\": \"CreateAsset\", \n"
      "            \"asset_name\": \"usd\", \n"
      "            \"domain_id\": \"test\", \n"
      "            \"precision\": \"2\"\n"
      "        }, \n"
      "        {\n"
      "            \"command_type\": \"AddAssetQuantity\",\n"
      "            \"account_id\": \"admin@test\",\n"
      "            \"asset_id\": \"usd#test\",\n"
      "            \"amount\": {\n"
      "                \"int_part\": -20,\n"
      "                \"frac_part\": 0\n"
      "            }\n"
      "        }\n"
      "    ]\n"
      "}";
  auto json = stringToJson(transaction_string);

  ASSERT_TRUE(json.has_value());

  auto transaction = factory.deserialize(json.value());

  ASSERT_FALSE(transaction.has_value());
}
