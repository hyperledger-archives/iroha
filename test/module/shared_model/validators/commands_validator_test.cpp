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
#include "builders/protobuf/transaction.hpp"
#include "utils/polymorphic_wrapper.hpp"

iroha::protocol::Transaction generateEmptyTransaction() {
  auto created_time = 10000000000ull;
  shared_model::interface::Transaction::TxCounterType tx_counter = 1;
  std::string creator_account_id = "admin@test";

  iroha::protocol::Transaction proto_tx;
  auto &payload = *proto_tx.mutable_payload();
  payload.set_tx_counter(tx_counter);
  payload.set_creator_account_id(creator_account_id);
  payload.set_created_time(created_time);
  return proto_tx;
}

iroha::protocol::AddAssetQuantity generateAddAssetQuantity(
    std::string account_id, std::string asset_id) {
  iroha::protocol::AddAssetQuantity command;

  command.set_account_id(account_id);
  command.set_asset_id(asset_id);
  command.mutable_amount()->mutable_value()->set_fourth(1000);
  command.mutable_amount()->set_precision(2);

  return command;
}

iroha::protocol::AddPeer generateAddPeer(std::string address,
                                         std::string peer_key) {
  iroha::protocol::AddPeer command;

  command.set_address(address);
  command.set_peer_key(peer_key);

  return command;
}

iroha::protocol::AddSignatory generateAddSignatory(std::string account_id,
                                                   std::string public_key) {
  iroha::protocol::AddSignatory command;

  command.set_account_id(account_id);
  command.set_public_key(public_key);

  return command;
}

iroha::protocol::CreateAsset generateCreateAsset(std::string asset_name,
                                                 std::string domain_id,
                                                 uint32_t precision) {
  iroha::protocol::CreateAsset command;

  command.set_asset_name(asset_name);
  command.set_domain_id(domain_id);
  command.set_precision(precision);

  return command;
}

iroha::protocol::CreateDomain generateCreateDomain(std::string domain_id,
                                                   std::string default_role) {
  iroha::protocol::CreateDomain command;

  command.set_domain_id(domain_id);
  command.set_default_role(default_role);

  return command;
}

iroha::protocol::RemoveSignatory generateRemoveSignatory(
    std::string account_id, std::string public_key) {
  iroha::protocol::RemoveSignatory command;

  command.set_account_id(account_id);
  command.set_public_key(public_key);

  return command;
}

iroha::protocol::SetAccountQuorum generateSetAccountQuorum(
    std::string account_id, uint32_t quorum) {
  iroha::protocol::SetAccountQuorum command;

  command.set_account_id(account_id);
  command.set_quorum(quorum);

  return command;
}

iroha::protocol::TransferAsset generateTransferAsset(
    std::string src_account_id,
    std::string dest_account_id,
    std::string asset_id,
    std::string description,
    std::string amount) {
  iroha::protocol::TransferAsset command;

  command.set_src_account_id(src_account_id);
  command.set_dest_account_id(dest_account_id);
  command.set_asset_id(asset_id);
  command.set_description(description);

  auto iroha_amount = iroha::Amount::createFromString(amount).value();
  auto proto_amount = command.mutable_amount();
  proto_amount->set_precision(iroha_amount.getPrecision());
  auto proto_amount_value = proto_amount->mutable_value();
  auto uint64s = iroha_amount.to_uint64s();
  proto_amount_value->set_first(uint64s.at(0));
  proto_amount_value->set_second(uint64s.at(1));
  proto_amount_value->set_third(uint64s.at(2));
  proto_amount_value->set_fourth(uint64s.at(3));

  return command;
}

iroha::protocol::AppendRole generateAppendRole(std::string account_id,
                                               std::string role_name) {
  iroha::protocol::AppendRole command;

  command.set_account_id(account_id);
  command.set_role_name(role_name);

  return command;
}

iroha::protocol::CreateRole generateCreateRole(
    std::string role_name,
    std::vector<iroha::protocol::RolePermission> permissions) {
  iroha::protocol::CreateRole command;

  command.set_role_name(role_name);
  for (auto permission : permissions) {
    command.add_permissions(permission);
  }

  return command;
}

iroha::protocol::GrantPermission generateGrantPermission(
    std::string account_id, iroha::protocol::GrantablePermission permission) {
  iroha::protocol::GrantPermission command;

  command.set_account_id(account_id);
  command.set_permission(permission);

  return command;
}

iroha::protocol::RevokePermission generateRevokePermission(
    std::string account_id, iroha::protocol::GrantablePermission permission) {
  iroha::protocol::RevokePermission command;

  command.set_account_id(account_id);
  command.set_permission(permission);

  return command;
}

using namespace iroha::protocol;
using namespace shared_model;

TEST(CommandsValidatorTest, StatelessInvalidTest) {
  iroha::protocol::Transaction tx = generateEmptyTransaction();
  auto payload = tx.mutable_payload();

  std::string invalid_account_id = "invalid_account_id";
  std::string invalid_asset_id = "invalid_asset_id";

  payload->add_commands()->mutable_add_asset_quantity()->CopyFrom(AddAssetQuantity());
  payload->add_commands()->mutable_add_peer()->CopyFrom(AddPeer());
  payload->add_commands()->mutable_add_signatory()->CopyFrom(AddSignatory());
  payload->add_commands()->mutable_append_role()->CopyFrom(AppendRole());
  payload->add_commands()->mutable_create_account()->CopyFrom(CreateAccount());
  payload->add_commands()->mutable_create_domain()->CopyFrom(CreateDomain());
  payload->add_commands()->mutable_create_role()->CopyFrom(CreateRole());
  payload->add_commands()->mutable_grant_permission()->CopyFrom(GrantPermission());
  payload->add_commands()->mutable_remove_sign()->CopyFrom(RemoveSignatory());
  payload->add_commands()->mutable_revoke_permission()->CopyFrom(RevokePermission());
  payload->add_commands()->mutable_set_quorum()->CopyFrom(SetAccountQuorum());
  payload->add_commands()->mutable_transfer_asset()->CopyFrom(TransferAsset());

  shared_model::validation::CommandsValidator commands_validator;
  auto answer = commands_validator.validate(detail::make_polymorphic<proto::Transaction>(tx));

  ASSERT_EQ(answer.getReasonsMap().size(), 12);
}
