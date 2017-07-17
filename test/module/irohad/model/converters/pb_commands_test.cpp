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
#include "model/commands/add_peer.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/assign_master_key.hpp"
#include "model/commands/create_account.hpp"


#include "model/converters/pb_command_factory.hpp"

TEST(CommandTest, add_peer) {
  auto orig_addPeer = iroha::model::AddPeer();
  // addPeer.peer_key =
  orig_addPeer.address = "10.90.129.23";

  auto factory = iroha::model::converters::PbCommandFactory();

  auto proto_add_peer = factory.serializeAddPeer(orig_addPeer);
  auto serial_addPeer = factory.deserializeAddPeer(proto_add_peer);

  // ASSERT_EQ(add_peer.peer_key, addPeer.peer_key);

  orig_addPeer.address = "134";
  //ASSERT_NE(serial_addPeer, orig_addPeer);
}

TEST(CommandTest, add_signatory) {
  auto orig_command = iroha::model::AddSignatory();
  orig_command.account_id = "23";


  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddSignatory(orig_command);
  auto serial_command = factory.deserializeAddSignatory(proto_command);

  //ASSERT_EQ(orig_command, serial_command);
}

TEST(CommandTest, add_asset_quantity) {
  auto orig_command = iroha::model::AddAssetQuantity();
  orig_command.account_id = "23";
  orig_command.amount = "1.50";
  orig_command.asset_id = "23";

  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddAssetQuantity(orig_command);
  auto serial_command = factory.deserializeAddAssetQuantity(proto_command);

  //ASSERT_EQ(orig_command, serial_command);
}


TEST(CommandTest, assign_master_key) {
  auto orig_command = iroha::model::AssignMasterKey();
  orig_command.account_id = "23";


  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAssignMasterKey(orig_command);
  auto serial_command = factory.deserializeAssignMasterKey(proto_command);

  //ASSERT_EQ(orig_command, serial_command);
}

TEST(CommandTest, AddSignatory) {
  auto orig_command = iroha::model::AddSignatory();
  orig_command.account_id = "23";


  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddSignatory(orig_command);
  auto serial_command = factory.deserializeAddSignatory(proto_command);

  //ASSERT_EQ(orig_command, serial_command);
}

TEST(CommandTest, AddSignatory) {
  auto orig_command = iroha::model::AddSignatory();
  orig_command.account_id = "23";


  auto factory = iroha::model::converters::PbCommandFactory();
  auto proto_command = factory.serializeAddSignatory(orig_command);
  auto serial_command = factory.deserializeAddSignatory(proto_command);

  //ASSERT_EQ(orig_command, serial_command);
}
