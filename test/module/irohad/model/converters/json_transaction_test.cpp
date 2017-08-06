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

using namespace iroha;
using namespace iroha::model;
using namespace iroha::model::converters;

class JsonTransactionTest : public ::testing::Test {
 public:
  JsonTransactionFactory factory;
};

TEST_F(JsonTransactionTest, ValidWhenWellFormed){
  Transaction transaction;

  auto json_transaction = factory.serialize(transaction);
  auto serial_transaction = factory.deserialize(json_transaction);

  ASSERT_EQ(transaction, serial_transaction);
}

TEST_F(JsonTransactionTest, InvalidWhenFieldsMissing){
  Transaction transaction;

  auto json_transaction = factory.serialize(transaction);
  
  json_transaction.RemoveMember("created_ts");
  
  auto serial_transaction = factory.deserialize(json_transaction);

  ASSERT_FALSE(serial_transaction.has_value());
}
