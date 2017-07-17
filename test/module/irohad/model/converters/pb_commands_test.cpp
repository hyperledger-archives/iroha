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
#include "commands.pb.h"
#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/assign_master_key.hpp"
#include "model/commands/create_account.hpp"

#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"
#include "model/commands/remove_signatory.hpp"
#include "model/commands/set_permissions.hpp"
#include "model/commands/set_quorum.hpp"
#include "model/commands/transfer_asset.hpp"

#include "model/block.hpp"
#include "model/transaction.hpp"

#include "model/converters/pb_block_factory.hpp"
#include "model/converters/pb_command_factory.hpp"
#include "model/converters/pb_transaction_factory.hpp"

TEST(CommandTest, add_peer) {
  auto orig_addPeer = iroha::model::AddPeer();
  // addPeer.peer_key =
  orig_addPeer.address = "10.90.129.23";

  auto factory = iroha::model::converters::PbCommandFactory();

  auto proto_add_peer = factory.serializeAddPeer(orig_addPeer);
  auto serial_addPeer = factory.deserializeAddPeer(proto_add_peer);

  ASSERT_EQ(orig_addPeer, serial_addPeer);

  orig_addPeer.address = "134";
  ASSERT_NE(serial_addPeer, orig_addPeer);
}

TEST(CommandTest, add_signatory) {
  auto orig_command = iroha::model::AddSignatory();
  orig_command.account_id = "23";

  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddSignatory(orig_command);
  auto serial_command = factory.deserializeAddSignatory(proto_command);

  ASSERT_EQ(orig_command, serial_command);
}

TEST(CommandTest, add_asset_quantity) {
  auto orig_command = iroha::model::AddAssetQuantity();
  orig_command.account_id = "23";
  iroha::Amount amount;
  amount.frac_part = 50;
  amount.int_part = 1;

  orig_command.amount = amount;
  orig_command.asset_id = "23";

  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddAssetQuantity(orig_command);
  auto serial_command = factory.deserializeAddAssetQuantity(proto_command);

  ASSERT_EQ(orig_command, serial_command);
}

TEST(CommandTest, assign_master_key) {
  auto orig_command = iroha::model::AssignMasterKey();
  orig_command.account_id = "23";
  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAssignMasterKey(orig_command);
  auto serial_command = factory.deserializeAssignMasterKey(proto_command);

  ASSERT_EQ(orig_command, serial_command);
}

TEST(CommandTest, create_account) {
  auto orig_command = iroha::model::CreateAccount();
  orig_command.account_name = "keker";
  orig_command.domain_id = "cheburek";
  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeCreateAccount(orig_command);
  auto serial_command = factory.deserializeCreateAccount(proto_command);
  ASSERT_EQ(orig_command, serial_command);
}

TEST(TransactionTest, tx_test) {
  auto orig_tx = iroha::model::Transaction();
  orig_tx.creator_account_id = "andr@kek";
  auto siga = iroha::model::Signature();
  std::fill(siga.pubkey.begin(), siga.pubkey.end(), 0x22);
  std::fill(siga.signature.begin(), siga.signature.end(), 0x10);
  orig_tx.signatures = {siga};

  orig_tx.created_ts = 2;
  orig_tx.tx_counter = 1;

  auto c1 = iroha::model::CreateDomain();
  c1.domain_name = "keker";
  auto c2 = iroha::model::CreateAsset();
  c2.domain_id = "keker";
  c2.precision = 2;
  c2.asset_name = "fedor-coin";

  auto c3 = iroha::model::SetAccountPermissions();
  c3.account_id = "fedor";
  c3.new_permissions.can_transfer = true;
  c3.new_permissions.create_assets = true;

  orig_tx.commands = {
      std::make_shared<iroha::model::CreateDomain>(c1),
      std::make_shared<iroha::model::CreateAsset>(c2),
      std::make_shared<iroha::model::SetAccountPermissions>(c3)};

  auto factory = iroha::model::converters::PbTransactionFactory();
  auto proto_tx = factory.serialize(orig_tx);
  auto serial_tx = factory.deserialize(proto_tx);
  ASSERT_EQ(orig_tx, serial_tx);
}

TEST(BlockTest, bl_test) {
  auto orig_tx = iroha::model::Transaction();
  orig_tx.creator_account_id = "andr@kek";
  auto siga = iroha::model::Signature();
  std::fill(siga.pubkey.begin(), siga.pubkey.end(), 0x22);
  std::fill(siga.signature.begin(), siga.signature.end(), 0x10);
  orig_tx.signatures = {siga};

  orig_tx.created_ts = 2;
  orig_tx.tx_counter = 1;

  auto c1 = iroha::model::CreateDomain();
  c1.domain_name = "keker";
  auto c2 = iroha::model::CreateAsset();
  c2.domain_id = "keker";
  c2.precision = 2;
  c2.asset_name = "fedor-coin";

  auto c3 = iroha::model::SetAccountPermissions();
  c3.account_id = "fedor";
  c3.new_permissions.can_transfer = true;
  c3.new_permissions.create_assets = true;

  orig_tx.commands = {
      std::make_shared<iroha::model::CreateDomain>(c1),
      std::make_shared<iroha::model::CreateAsset>(c2),
      std::make_shared<iroha::model::SetAccountPermissions>(c3)};

  auto orig_block = iroha::model::Block();
  orig_block.created_ts = 1;
  std::fill(orig_block.hash.begin(), orig_block.hash.end(), 0x7);

  std::fill(orig_block.prev_hash.begin(), orig_block.prev_hash.end(), 0x3);
  orig_block.sigs = {siga};

  std::fill(orig_block.merkle_root.begin(), orig_block.merkle_root.end(), 0x14);
  orig_block.height = 3;
  orig_block.txs_number = 1;
  orig_block.transactions = {orig_tx};

  auto factory = iroha::model::converters::PbBlockFactory();
  auto proto_block = factory.serialize(orig_block);
  auto serial_block = factory.deserialize(proto_block);
  ASSERT_EQ(orig_block, serial_block);
}