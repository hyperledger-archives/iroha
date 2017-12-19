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
#include <boost/range/irange.hpp>
#include "builders/protobuf/transaction.hpp"
#include "utils/polymorphic_wrapper.hpp"

// TODO kamilsa 08.12.2017 IR-701 improve transaction builder api, so that we
// can omit generate methods below

iroha::protocol::Transaction generateEmptyTransaction() {
  auto created_time = 10000000000ull;
  shared_model::interface::types::CounterType tx_counter = 1;
  std::string creator_account_id = "admin@test";

  iroha::protocol::Transaction proto_tx;
  auto &payload = *proto_tx.mutable_payload();
  payload.set_tx_counter(tx_counter);
  payload.set_creator_account_id(creator_account_id);
  payload.set_created_time(created_time);
  return proto_tx;
}

using namespace iroha::protocol;
using namespace shared_model;

/**
 * @given transaction without any commands
 * @when commands validator is invoked
 * @then answer has error about empty transaction
 */
TEST(commandsValidatorTest, EmptyTransactionTest) {
  auto tx = generateEmptyTransaction();
  tx.mutable_payload()->set_created_time(iroha::time::now());
  shared_model::validation::CommandsValidator commands_validator;
  auto answer = commands_validator.validate(
      detail::make_polymorphic<proto::Transaction>(tx));
  ASSERT_EQ(answer.getReasonsMap().size(), 1);
}

/**
 * @given transaction made of commands with valid fields
 * @when commands validation is invoked
 * @then answer has no errors
 */
TEST(CommandsValidatorTest, StatelessValidTest) {
  std::string valid_account_id = "account@domain";
  std::string valid_asset_id = "asset#domain";
  std::string valid_address = "localhost";
  iroha::protocol::Amount valid_amount;
  valid_amount.set_precision(2);
  valid_amount.mutable_value()->set_fourth(1000);
  auto public_key_size = 32;
  std::string valid_public_key(public_key_size, '0');
  std::string valid_role_name = "user";
  std::string valid_account_name = "admin";
  std::string valid_domain_id = "ru";
  std::string valid_asset_name = "asset";
  uint8_t valid_precision = 42;
  iroha::protocol::RolePermission valid_role_permission =
      iroha::protocol::RolePermission::can_append_role;
  iroha::protocol::GrantablePermission valid_grantable_permission =
      iroha::protocol::GrantablePermission::can_add_my_signatory;
  std::string valid_detail_key = "key";
  uint8_t valid_quorum = 2;
  auto valid_created_time = iroha::time::now();

  auto setField = [&](auto setter) {
    return [setter](const auto &value) {
      return [setter, &value](auto refl, auto msg, auto field) {
        (refl->*setter)(msg, field, value);
      };
    };
  };

  auto setString = setField(&google::protobuf::Reflection::SetString);
  auto setUInt32 = setField(&google::protobuf::Reflection::SetUInt32);
  auto addEnum = setField(&google::protobuf::Reflection::AddEnumValue);
  auto setEnum = setField(&google::protobuf::Reflection::SetEnumValue);

  std::unordered_map<std::string,
                     std::function<void(
                         const google::protobuf::Reflection *,
                         google::protobuf::Message *,
                         const google::protobuf::FieldDescriptor *)>>
      field_setters;
  for (const auto &id : {"account_id", "src_account_id", "dest_account_id"}) {
    field_setters[id] = setString(valid_account_id);
  }
  for (const auto &id : {"peer_key", "public_key", "main_pubkey"}) {
    field_setters[id] = setString(valid_public_key);
  }
  for (const auto &id : {"role_name", "default_role"}) {
    field_setters[id] = setString(valid_role_name);
  }
  field_setters["asset_id"] = setString(valid_asset_id);
  field_setters["address"] = setString(valid_address);
  field_setters["account_name"] = setString(valid_account_name);
  field_setters["domain_id"] = setString(valid_domain_id);
  field_setters["asset_name"] = setString(valid_asset_name);
  field_setters["precision"] = setUInt32(valid_precision);
  field_setters["permissions"] = addEnum(valid_role_permission);
  field_setters["permission"] = setEnum(valid_grantable_permission);
  field_setters["key"] = setString(valid_detail_key);
  field_setters["value"] = setString("");
  field_setters["quorum"] = setUInt32(valid_quorum);
  field_setters["description"] = setString("");
  field_setters["amount"] = [&](auto refl, auto msg, auto field) {
    refl->MutableMessage(msg, field)->CopyFrom(valid_amount);
  };

  iroha::protocol::Transaction tx = generateEmptyTransaction();
  tx.mutable_payload()->set_creator_account_id(valid_account_id);
  tx.mutable_payload()->set_created_time(valid_created_time);
  auto payload = tx.mutable_payload();

  iroha::protocol::Command command;
  auto refl = command.GetReflection();
  auto desc = command.GetDescriptor();
  boost::for_each(boost::irange(0, desc->field_count()), [&](auto i) {
    auto field = desc->field(i);
    auto command = refl->MutableMessage(payload->add_commands(), field);

    auto command_refl = command->GetReflection();
    auto command_desc = command->GetDescriptor();

    boost::for_each(boost::irange(0, command_desc->field_count()), [&](auto i) {
      auto field = command_desc->field(i);

      field_setters.at(field->name())(command_refl, command, field);
    });
  });

  shared_model::validation::CommandsValidator commands_validator;
  auto answer = commands_validator.validate(
      detail::make_polymorphic<proto::Transaction>(tx));

  ASSERT_FALSE(answer.hasErrors());
}

/**
 * @given transaction made of commands with invalid fields
 * @when commands validation is invoked
 * @then answer has errors and number of errors in answer is the same as the
 * number of commands in tx
 */
TEST(CommandsValidatorTest, StatelessInvalidTest) {
  iroha::protocol::Transaction tx = generateEmptyTransaction();
  auto payload = tx.mutable_payload();

  // create commands from default constructors, which will have empty, therefore
  // invalid values
  iroha::protocol::Command command;
  auto refl = command.GetReflection();
  auto desc = command.GetDescriptor();
  boost::for_each(
      boost::irange(0, desc->field_count()),
      [&](auto i) {
        auto field = desc->field(i);
        refl->SetAllocatedMessage(
            &command, refl->GetMessage(command, field).New(), field);
        *payload->add_commands() = command;
      });

  shared_model::validation::CommandsValidator commands_validator;
  auto answer = commands_validator.validate(
      detail::make_polymorphic<proto::Transaction>(tx));

  // in total there should be number_of_commands + 1 reasons of bad answer:
  // number_of_commands for each command + 1 for transaction metadata
  ASSERT_EQ(answer.getReasonsMap().size(), desc->field_count() + 1);
}
