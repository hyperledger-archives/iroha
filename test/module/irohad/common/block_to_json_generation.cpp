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
#include "ametsuchi/block_serializer.hpp"
#include "model/block.hpp"
#include "model/transaction.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/transfer_asset.hpp"
#include "common/blob_converter.hpp"
#include <iostream>

using namespace iroha::ametsuchi;
using namespace iroha::model;
using namespace std;

Transaction getTransaction(uint64_t create_time, std::string address) {
  Transaction transaction;
  transaction.created_ts = create_time;

  AddPeer add_peer;
  add_peer.address = std::move(address);

  TransferAsset transfer_asset;
  transfer_asset.amount = iroha::Amount(69, 42);
  transfer_asset.asset_id = "tugrik";
  transfer_asset.src_account_id = "Tourist";
  transfer_asset.dest_account_id = "Petr";
  transaction.commands = {
      std::make_shared<AddPeer>(add_peer),
      std::make_shared<TransferAsset>(transfer_asset),
  };
  return transaction;
}

Block generateBlock() {
  Block block;
  block.created_ts = 100500;
  block.height = 228;

  block.transactions = {
      getTransaction(322, "ulitsa pushkina, dom kolotushkina"),
      getTransaction(666, "Petushki"),

  };
  return block;
}

TEST(JsonRepr, JsonBlockReprGeneration) {
  cout << "----------| generate block => print |----------" << endl;

  BlockSerializer serializer;
  auto blob = serializer.serialize(generateBlock());
  cout << iroha::common::convert(blob) << endl;
}

TEST(BlockInsertionTest, BlockInsertionWhenParseBlock) {
  cout << "----------| block => string_repr(block)"
      " => parseBlock() |----------" << endl;

  auto block = generateBlock();
}
