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
#include "model/converters/pb_query_response_factory.hpp"

using namespace iroha;

TEST(QueryResponseTest, AccountTest) {
  model::converters::PbQueryResponseFactory pb_factory;
  model::Account account;
  account.account_id = "123";
  std::fill(account.master_key.begin(), account.master_key.end(), 0x1);
  account.permissions.add_signatory = true;
  account.domain_name = "domain";
  account.quorum = 32;

  auto pb_account = pb_factory.serialize(account);
  auto des_account = pb_factory.deserialize(pb_account);

  ASSERT_EQ(account.account_id, des_account.account_id);
  ASSERT_EQ(account.master_key, des_account.master_key);
  ASSERT_EQ(account.permissions, des_account.permissions);
  ASSERT_EQ(account.domain_name, des_account.domain_name);
  ASSERT_EQ(account.quorum, account.quorum);
}

TEST(QueryResponseTest, AccountResponseTest) {
  model::converters::PbQueryResponseFactory pb_factory;

  model::Account account;
  account.account_id = "123";
  std::fill(account.master_key.begin(), account.master_key.end(), 0x1);
  account.permissions.add_signatory = true;
  account.domain_name = "domain";
  account.quorum = 32;

  model::AccountResponse accountResponse;
  accountResponse.account = account;

  auto pb_account_response = pb_factory.serialize(accountResponse);
  auto des_account_response = pb_factory.deserialize(pb_account_response);

  ASSERT_EQ(accountResponse.account.account_id,
            des_account_response.account.account_id);
  ASSERT_EQ(accountResponse.account.master_key,
            des_account_response.account.master_key);
  ASSERT_EQ(accountResponse.account.permissions,
            des_account_response.account.permissions);
  ASSERT_EQ(accountResponse.account.domain_name,
            des_account_response.account.domain_name);
  ASSERT_EQ(accountResponse.account.quorum, account.quorum);
}