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
#include <nonstd/optional.hpp>

#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/assign_master_key.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/remove_signatory.hpp"
#include "model/commands/set_permissions.hpp"

auto ADMIN_ID = "admin@test";
auto ACCOUNT_ID = "test@test";
auto ASSET_ID = "coin@test";
auto TARGET_ID = "target@test";
auto DOMAIN_NAME = "test";
auto BALANCE = 150ul;

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;
using ::testing::AllOf;

iroha::model::Account get_default_creator() {
  iroha::model::Account creator = iroha::model::Account();
  creator.account_id = ADMIN_ID;
  creator.domain_name = DOMAIN_NAME;
  std::fill(creator.master_key.begin(), creator.master_key.end(), 0x1);
  creator.quorum = 1;

  return creator;
}

iroha::model::Account get_default_account() {
  auto dummy = iroha::model::Account();
  dummy.account_id = ACCOUNT_ID;
  dummy.domain_name = DOMAIN_NAME;
  std::fill(dummy.master_key.begin(), dummy.master_key.end(), 0x2);
  dummy.quorum = 1;
  return dummy;
}

iroha::model::Account get_default_target() {
  auto dummy = iroha::model::Account();
  dummy.account_id = TARGET_ID;
  dummy.domain_name = DOMAIN_NAME;
  std::fill(dummy.master_key.begin(), dummy.master_key.end(), 0x3);
  dummy.quorum = 1;
  return dummy;
}

iroha::model::Asset get_default_asset() {
  auto asset = iroha::model::Asset();
  asset.asset_id = ASSET_ID;
  asset.domain_id = DOMAIN_NAME;
  asset.precision = 2;
  return asset;
}

class WSVQueriesMock : public iroha::ametsuchi::WsvQuery {
 public:
  MOCK_METHOD1(getAccount, nonstd::optional<iroha::model::Account>(
                               const std::string &account_id));
  MOCK_METHOD1(getSignatories, std::vector<iroha::ed25519::pubkey_t>(
                                   const std::string &account_id));
  MOCK_METHOD1(getAsset, nonstd::optional<iroha::model::Asset>(
                             const std::string &asset_id));

  MOCK_METHOD2(getAccountAsset,
               nonstd::optional<iroha::model::AccountAsset>(
                   const std::string &account_id, const std::string &asset_id));
  MOCK_METHOD0(getPeers, std::vector<iroha::model::Peer>());
};

class WSVCommandsMock : public iroha::ametsuchi::WsvCommand {
 public:
  MOCK_METHOD1(insertAccount, bool(const iroha::model::Account &));
  MOCK_METHOD1(updateAccount, bool(const iroha::model::Account &));
  MOCK_METHOD1(insertAsset, bool(const iroha::model::Asset &));
  MOCK_METHOD1(upsertAccountAsset, bool(const iroha::model::AccountAsset &));
  MOCK_METHOD1(insertSignatory, bool(const iroha::ed25519::pubkey_t &));

  MOCK_METHOD2(insertAccountSignatory,
               bool(const std::string &, const iroha::ed25519::pubkey_t &));

  MOCK_METHOD2(deleteAccountSignatory,
               bool(const std::string &, const iroha::ed25519::pubkey_t &));

  MOCK_METHOD1(insertPeer, bool(const iroha::model::Peer &));

  MOCK_METHOD1(deletePeer, bool(const iroha::model::Peer &));

  MOCK_METHOD1(insertDomain, bool(const iroha::model::Domain &));
};

void set_default_wsv(WSVQueriesMock &test_wsv, WSVCommandsMock &test_commands) {
  // No account exist
  EXPECT_CALL(test_wsv, getAccount(_)).WillRepeatedly(Return(nonstd::nullopt));
  // Admin exist
  auto admin = get_default_creator();

  EXPECT_CALL(test_wsv, getAccount(ADMIN_ID)).WillRepeatedly(Return(admin));
  // Test account exist
  auto dummy = get_default_account();

  EXPECT_CALL(test_wsv, getAccount(ACCOUNT_ID)).WillRepeatedly(Return(dummy));

  auto target_dummy = get_default_target();
  EXPECT_CALL(test_wsv, getAccount(TARGET_ID))
      .WillRepeatedly(Return(target_dummy));

  EXPECT_CALL(test_wsv, getAsset(_)).WillRepeatedly(Return(nonstd::nullopt));
  auto asset = get_default_asset();

  EXPECT_CALL(test_wsv, getAsset(ASSET_ID)).WillRepeatedly(Return(asset));

  EXPECT_CALL(test_commands, upsertAccountAsset(_))
      .WillRepeatedly(Return(true));

  EXPECT_CALL(test_commands, updateAccount(_)).WillRepeatedly(Return(true));

  EXPECT_CALL(test_commands, insertAsset(_)).WillRepeatedly(Return(true));
}

TEST(CommandValidation, add_asset_quantity) {
  WSVQueriesMock test_wsv;
  WSVCommandsMock test_commands;

  auto creator = get_default_creator();

  iroha::model::AddAssetQuantity command;
  command.account_id = ACCOUNT_ID;
  command.amount.int_part = 3;
  command.amount.frac_part = 50;
  command.asset_id = ASSET_ID;

  set_default_wsv(test_wsv, test_commands);

  iroha::model::AccountAsset wallet;
  wallet.asset_id = ASSET_ID;
  wallet.account_id = ACCOUNT_ID;
  wallet.balance = BALANCE;
  // Valid cases:
  // Case 1. Add asset first time - no wallet
  // Case 2. There is already asset- there is a wallet
  EXPECT_CALL(test_wsv, getAccountAsset(ACCOUNT_ID, ASSET_ID))
      .WillOnce(Return(nonstd::nullopt))
      .WillOnce(Return(wallet))
      .WillRepeatedly(Return(nonstd::nullopt));

  creator.permissions.issue_assets = true;

  // Case 1. When there is no wallet - new accountAsset will be created
  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  // Case 2. When there is a wallet - no new accountAsset created
  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  // Non valid cases:
  // 1. Creator has no permission
  creator.permissions.issue_assets = false;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 2. Amount is zero
  creator.permissions.issue_assets = true;
  command.amount.int_part = 0;
  command.amount.frac_part = 0;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 3. Amount is with wrong precision (must be 2)
  creator.permissions.issue_assets = true;
  command.amount.int_part = 0;
  command.amount.frac_part = 300;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 4. Account to add doesn't exist
  command.amount.int_part = 1;
  command.amount.frac_part = 50;
  command.account_id = "noacc";

  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 5. Asset doesn't exist
  command.account_id = ACCOUNT_ID;
  command.asset_id = "noass";

  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));
}

TEST(CommandValidation, add_signatory) {
  WSVQueriesMock test_wsv;
  WSVCommandsMock test_commands;

  auto creator = get_default_creator();

  iroha::model::AddSignatory command;
  command.account_id = ACCOUNT_ID;
  std::fill(command.pubkey.begin(), command.pubkey.end(),
            0x1);  // Such Pubkey exist

  set_default_wsv(test_wsv, test_commands);

  // Valid cases:
  // 1. Creator has permissions
  creator.permissions.add_signatory = true;

  EXPECT_CALL(test_commands, insertAccountSignatory(_, _))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(test_commands, insertAccountSignatory(ACCOUNT_ID, command.pubkey))
      .WillRepeatedly(Return(true));

  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  // 2. When creator is adding public keys to his account
  EXPECT_CALL(test_commands,
              insertAccountSignatory(ACCOUNT_ID, creator.master_key))
      .WillRepeatedly(Return(true));

  command.pubkey = creator.master_key;
  creator = get_default_account();

  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  //--------- Non valid cases:--------------
  // 1. Creator has no permission
  creator = get_default_creator();
  creator.permissions.add_signatory = false;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 2. Trying  to add non existed public key
  creator.permissions.add_signatory = true;
  std::fill(command.pubkey.begin(), command.pubkey.end(), 0xF);
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 3. Add to non existed account
  command.pubkey = creator.master_key;
  command.account_id = "noacc";
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 4. Add same signatory
  auto account = get_default_account();
  command.pubkey = account.master_key;
  command.account_id = ACCOUNT_ID;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));
}

TEST(CommandValidation, assign_master_key) {
  WSVQueriesMock test_wsv;
  WSVCommandsMock test_commands;

  auto creator = get_default_creator();
  auto orig_account = get_default_account();

  iroha::model::AssignMasterKey command;
  command.account_id = ACCOUNT_ID;
  set_default_wsv(test_wsv, test_commands);

  std::vector<iroha::ed25519::pubkey_t> sigs = {orig_account.master_key,
                                                creator.master_key};

  EXPECT_CALL(test_wsv, getSignatories(_))
      .WillRepeatedly(Return(std::vector<iroha::ed25519::pubkey_t>{}));

  EXPECT_CALL(test_wsv, getSignatories(ACCOUNT_ID))
      .WillRepeatedly(Return(sigs));

  command.pubkey = creator.master_key;
  // Valid cases:
  // 1. Creator is sys admin

  creator.permissions.add_signatory = true;

  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  // 2. Creator is account itself
  creator = orig_account;
  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  //--------- Non valid cases:--------------
  // 1. Creator has no permissions
  creator = get_default_creator();
  creator.permissions.add_signatory = false;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 2. Assign random master key
  creator.permissions.add_signatory = true;
  std::fill(command.pubkey.begin(), command.pubkey.end(), 0xF);
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 3. Add to non existed account
  command.pubkey = creator.master_key;
  command.account_id = "noacc";
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));
}

TEST(CommandValidation, create_account) {
  WSVQueriesMock test_wsv;
  WSVCommandsMock test_commands;

  auto creator = get_default_creator();
  // Valid case
  creator.permissions.create_accounts = true;
  iroha::model::CreateAccount createAccount;
  std::fill(createAccount.pubkey.begin(), createAccount.pubkey.end(), 0x2);
  createAccount.account_name = "test";
  createAccount.domain_id = "test";

  EXPECT_CALL(test_commands, insertSignatory(createAccount.pubkey))
      .Times(1)
      .WillOnce(Return(true));

  EXPECT_CALL(test_commands, insertAccount(_)).Times(1).WillOnce(Return(true));

  EXPECT_CALL(test_commands, insertAccountSignatory(_, _))
      .Times(1)
      .WillOnce(Return(true));

  ASSERT_TRUE(createAccount.validate(test_wsv, creator));
  ASSERT_TRUE(createAccount.execute(test_wsv, test_commands));

  // Non valid cases:
  // 1. Creator has no permission

  creator.permissions.create_accounts = false;
  ASSERT_FALSE(createAccount.validate(test_wsv, creator));

  // 2. Not valid name for account
  creator.permissions.create_accounts = true;
  createAccount.account_name = "thisisaverybigname";
  ASSERT_FALSE(createAccount.validate(test_wsv, creator));

  // 3. Not valid name for account (system symbols)
  createAccount.account_name = "test@";
  ASSERT_FALSE(createAccount.validate(test_wsv, creator));
}

TEST(CommandValidation, create_asset) {
  WSVQueriesMock test_wsv;
  WSVCommandsMock test_commands;

  auto creator = get_default_creator();

  iroha::model::CreateAsset command;

  set_default_wsv(test_wsv, test_commands);

  // Valid cases:
  // 1. Creator is money creator
  creator.permissions.create_assets = true;
  command.precision = 2;
  command.domain_id = DOMAIN_NAME;
  command.asset_name = "FCoin";

  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  //--------- Non valid cases:--------------
  // 1. Creator has no permissions
  creator = get_default_creator();
  creator.permissions.add_signatory = false;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));
}

TEST(CommandValidation, remove_signatory) {
  WSVQueriesMock test_wsv;
  WSVCommandsMock test_commands;

  auto creator = get_default_creator();
  auto orig_account = get_default_account();

  iroha::model::RemoveSignatory command;

  set_default_wsv(test_wsv, test_commands);

  EXPECT_CALL(test_commands, deleteAccountSignatory(_, _))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(test_commands,
              deleteAccountSignatory(ACCOUNT_ID, orig_account.master_key))
      .WillRepeatedly(Return(true));

  EXPECT_CALL(test_commands,
              deleteAccountSignatory(ACCOUNT_ID, creator.master_key))
      .WillRepeatedly(Return(true));

  // Valid cases:
  // 1. Creator is admin
  creator.permissions.remove_signatory = true;
  command.account_id = ACCOUNT_ID;
  command.pubkey = creator.master_key;

  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  //--------- Non valid cases:--------------
  // 1. Creator has no permissions
  creator.permissions.remove_signatory = false;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 2.Remove master key
  creator.permissions.remove_signatory = true;
  command.pubkey = orig_account.master_key;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 3. Remove signatory not present in account
  std::fill(command.pubkey.begin(), command.pubkey.end(), 0xF);
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));
}

TEST(CommandValidation, set_account_permissions) {
  WSVQueriesMock test_wsv;
  WSVCommandsMock test_commands;

  auto creator = get_default_creator();
  auto orig_account = get_default_account();

  iroha::model::SetAccountPermissions command;

  set_default_wsv(test_wsv, test_commands);
  // Valid cases:
  // 1. Creator is admin
  creator.permissions.set_permissions = true;
  command.account_id = ACCOUNT_ID;
  command.new_permissions.create_assets = true;

  ASSERT_TRUE(command.validate(test_wsv, creator) &&
              command.execute(test_wsv, test_commands));

  //--------- Non valid cases:--------------
  // 1. Creator has no permissions
  creator.permissions.set_permissions = false;
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));

  // 2.No such account exits
  creator.permissions.set_permissions = true;
  command.account_id = "noacc";
  ASSERT_FALSE(command.validate(test_wsv, creator) &&
               command.execute(test_wsv, test_commands));
}
