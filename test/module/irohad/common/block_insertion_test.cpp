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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "main/raw_block_insertion.hpp"
#include "module/irohad/ametsuchi/ametsuchi_mocks.hpp"

#include <fstream>
#include "model/converters/json_block_factory.hpp"
#include "model/converters/pb_command_factory.hpp"
#include "model/converters/json_common.hpp"
#include "framework/test_block_generator.hpp"
#include "logger/logger.hpp"

using ::testing::DefaultValue;
using ::testing::_;
using namespace iroha::main;
using namespace iroha::ametsuchi;
using namespace iroha::model;
using namespace iroha::model::converters;
using namespace iroha;
using namespace std;

MockMutableStorage *storage_mock = new MockMutableStorage();

std::unique_ptr<MutableStorage> createMutableStorageMock() {
  return unique_ptr<MockMutableStorage>(storage_mock);
}

bool save_to_file(std::string json, std::string file_name) {
  // Save pubkey to file
  std::ofstream file(file_name);
  if (not file) {
    return false;
  }
  file << json;
  return true;
}

TEST(JsonRepr, JsonBlockReprGeneration) {
  cout << "----------| generate block => print |----------" << endl;

  JsonBlockFactory serializer;
  auto blob = serializer.serialize(framework::generator::generateBlock());
  auto json_block = jsonToString(blob);

  cout << json_block << endl;
  ASSERT_TRUE(save_to_file(json_block, "zero.block"));
}

TEST(BlockInsertionTest, BlockInsertionWhenParseBlock) {
  cout << "----------| block => string_repr(block)"
          " => parseBlock() |----------"
       << endl;

  shared_ptr<MockMutableFactory> factory = make_shared<MockMutableFactory>();
  BlockInserter inserter(factory);
  auto block = framework::generator::generateBlock();
  auto doc = JsonBlockFactory().serialize(block);
  auto str = jsonToString(doc);
  auto new_block = inserter.parseBlock(str);
  ASSERT_TRUE(new_block.has_value());
  ASSERT_EQ(block, new_block.value());
}

TEST(BlockInsertionTest, BlockInsertionWhenApplyToStorage) {
  cout << "----------| block => applyToLedger() |----------" << endl;

  shared_ptr<MockMutableFactory> factory = make_shared<MockMutableFactory>();
  DefaultValue<std::unique_ptr<MutableStorage>>::SetFactory(
      &createMutableStorageMock);

  EXPECT_CALL(*factory, createMutableStorage()).Times(1);
  EXPECT_CALL(*factory, commit_(_)).Times(1);
  EXPECT_CALL(*storage_mock, apply(_, _)).Times(1);

  BlockInserter inserter(factory);
  auto block = framework::generator::generateBlock();
  vector<Block> blocks{block};

  inserter.applyToLedger(blocks);
}
