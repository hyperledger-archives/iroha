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

#include "module/irohad/ametsuchi/ametsuchi_mocks.hpp"

#include <model/queries/responses/account_assets_response.hpp>
#include "model/permissions.hpp"
#include "model/queries/responses/account_response.hpp"
#include "model/queries/responses/asset_response.hpp"
#include "model/queries/responses/error_response.hpp"
#include "model/queries/responses/roles_response.hpp"
#include "model/query_execution.hpp"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;
using ::testing::AllOf;
using ::testing::StrictMock;

using namespace iroha::ametsuchi;
using namespace iroha::model;

class QueryValidateExecuteTest : public ::testing::Test {
 public:
  QueryValidateExecuteTest() = default;

  void SetUp() override {
    wsv_query = std::make_shared<StrictMock<MockWsvQuery>>();
    block_query = std::make_shared<StrictMock<MockBlockQuery>>();
    factory = std::make_shared<QueryProcessingFactory>(wsv_query, block_query);

    creator.account_id = admin_id;
    creator.domain_id = domain_id;
    creator.json_data = "{}";
    creator.quorum = 1;

    account.account_id = account_id;
    account.domain_id = domain_id;
    account.json_data = "{}";
    account.quorum = 1;
  }

  std::shared_ptr<QueryResponse> validateAndExecute() {
    return factory->execute(query);
  }

  std::string admin_id = "admin@test", account_id = "test@test",
              asset_id = "coin#test", domain_id = "test";

  std::string admin_role = "admin";

  std::vector<std::string> admin_roles = {admin_role};
  std::vector<std::string> role_permissions;
  Domain default_domain;

  Account creator, account;
  std::shared_ptr<MockWsvQuery> wsv_query;
  std::shared_ptr<MockBlockQuery> block_query;

  std::shared_ptr<QueryProcessingFactory> factory;
  std::shared_ptr<Query> query;
};

class GetAccountTest : public QueryValidateExecuteTest {
 public:
  void SetUp() override {
    QueryValidateExecuteTest::SetUp();
    get_account = std::make_shared<GetAccount>();
    get_account->account_id = admin_id;
    get_account->creator_account_id = admin_id;
    query = get_account;

    role_permissions = {can_get_my_account};
  }
  std::shared_ptr<GetAccount> get_account;
};

/**
 * @given initialized storage, permission to his account
 * @when get account information
 * @then Return account
 */
TEST_F(GetAccountTest, MyAccountValidCase) {
  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillRepeatedly(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillRepeatedly(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getAccount(admin_id)).WillOnce(Return(creator));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountResponse>(response);
  ASSERT_EQ(cast_resp->account.account_id, admin_id);
}

/**
 * @given initialized storage, global permission
 * @when get account information about other user
 * @then Return account
 */
TEST_F(GetAccountTest, AllAccountValidCase) {
  get_account->account_id = account_id;
  get_account->creator_account_id = admin_id;
  role_permissions = {can_get_all_accounts};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillRepeatedly(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillRepeatedly(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getAccount(account_id)).WillOnce(Return(account));
  EXPECT_CALL(*wsv_query, getAccountRoles(account_id))
      .WillOnce(Return(admin_roles));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountResponse>(response);
  ASSERT_EQ(cast_resp->account.account_id, account_id);
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the same domain
 * @then Return account
 */
TEST_F(GetAccountTest, DomainAccountValidCase) {
  get_account->account_id = account_id;
  get_account->creator_account_id = admin_id;
  role_permissions = {can_get_domain_accounts};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillRepeatedly(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillRepeatedly(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getAccount(account_id)).WillOnce(Return(account));
  EXPECT_CALL(*wsv_query, getAccountRoles(account_id))
      .WillOnce(Return(admin_roles));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountResponse>(response);
  ASSERT_EQ(cast_resp->account.account_id, account_id);
}

/**
 * @given initialized storage, granted permission
 * @when get account information about other user
 * @then Return error
 */
TEST_F(GetAccountTest, GrantAccountValidCase) {
  get_account->account_id = account_id;
  get_account->creator_account_id = admin_id;
  role_permissions = {};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillRepeatedly(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillRepeatedly(Return(role_permissions));
  EXPECT_CALL(*wsv_query,
              hasAccountGrantablePermission(
                  admin_id, get_account->account_id, can_get_my_account))
      .WillOnce(Return(true));

  EXPECT_CALL(*wsv_query, getAccount(account_id)).WillOnce(Return(account));
  EXPECT_CALL(*wsv_query, getAccountRoles(account_id))
      .WillOnce(Return(admin_roles));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountResponse>(response);
  ASSERT_EQ(cast_resp->account.account_id, account_id);
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the other domain
 * @then Return error
 */
TEST_F(GetAccountTest, DifferentDomainAccountInValidCase) {
  get_account->account_id = "test@test2";  // other domain
  get_account->creator_account_id = admin_id;
  role_permissions = {can_get_domain_accounts};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillRepeatedly(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillRepeatedly(Return(role_permissions));
  EXPECT_CALL(*wsv_query,
              hasAccountGrantablePermission(
                  admin_id, get_account->account_id, can_get_my_account))
      .WillOnce(Return(false));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::ErrorResponse>(response);
  ASSERT_EQ(cast_resp->reason, ErrorResponse::STATEFUL_INVALID);
}

/**
 * @given initialized storage, permission
 * @when get account information about non existing account
 * @then Return error
 */
TEST_F(GetAccountTest, NoAccountExist) {
  get_account->account_id = "none";
  get_account->creator_account_id = admin_id;
  role_permissions = {can_get_all_accounts};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillRepeatedly(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillRepeatedly(Return(role_permissions));

  EXPECT_CALL(*wsv_query, getAccount(get_account->account_id))
      .WillOnce(Return(nonstd::nullopt));
  EXPECT_CALL(*wsv_query, getAccountRoles(get_account->account_id))
      .WillOnce(Return(nonstd::nullopt));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::ErrorResponse>(response);
  ASSERT_EQ(cast_resp->reason, ErrorResponse::NO_ACCOUNT);
}