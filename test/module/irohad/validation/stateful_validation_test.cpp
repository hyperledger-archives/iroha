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
#include <gmock/gmock.h>
#include <nonstd/optional.hpp>

#include "model/commands/create_account.hpp"
#include "ametsuchi/wsv_query.hpp"
#include "model/account.hpp"

iroha::model::Account get_default_creator(){
  iroha::model::Account creator;
  creator.account_id = "admin@test";
  creator.domain_name = "test";
  std::fill(creator.master_key.begin(),creator.master_key.end(), 0x1);
  creator.quorum = 1;
  return creator;
}

class WSVQueriesMock : public  iroha::ametsuchi::WsvQuery{
 public:
  MOCK_METHOD1(getAccount, nonstd::optional<model::Account>(const std::string &account_id));
  MOCK_METHOD1(getSignatories, std::vector<ed25519::pubkey_t>(const std::string &account_id));
  MOCK_METHOD1(getAsset, nonstd::optional<model::Asset>(const std::string &asset_id));

  MOCK_METHOD2(getAccountAsset, nonstd::optional<model::AccountAsset>(const std::string &account_id, const std::string &asset_id));
  MOCK_METHOD1(getPeers, nonstd::optional<model::Account>(const std::string &account_id));
};


TEST(CommandValidation, create_account){

  WSVQueriesMock test_wsv;

  auto creator = get_default_creator();
  // Valid case
  creator.permissions.create_accounts = true;
  iroha::model::CreateAccount createAccount;
  std::fill(createAccount.pubkey.begin(),createAccount.pubkey.end(), 0x2);
  createAccount.account_name = "test";
  createAccount.domain_id = "test";

  //EXPECT_CALL(test_wsv, getAccount(_));

  ASSERT_TRUE(createAccount.validate(test_wsv, creator));
}