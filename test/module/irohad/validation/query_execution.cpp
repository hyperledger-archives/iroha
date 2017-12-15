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
#include "model/queries/responses/signatories_response.hpp"
#include "model/queries/responses/transactions_response.hpp"
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

    EXPECT_CALL(*wsv_query, hasAccountGrantablePermission(_, _, _))
        .WillRepeatedly(Return(false));

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
      .Times(2)
      .WillRepeatedly(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
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
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
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
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
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
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
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
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
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
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));

  EXPECT_CALL(*wsv_query, getAccount(get_account->account_id))
      .WillOnce(Return(nonstd::nullopt));
  EXPECT_CALL(*wsv_query, getAccountRoles(get_account->account_id))
      .WillOnce(Return(nonstd::nullopt));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::ErrorResponse>(response);
  ASSERT_EQ(cast_resp->reason, ErrorResponse::NO_ACCOUNT);
}

/// --------- Get Account Assets -------------
class GetAccountAssetsTest : public QueryValidateExecuteTest {
 public:
  void SetUp() override {
    QueryValidateExecuteTest::SetUp();
    get_account_assets = std::make_shared<GetAccountAssets>();
    get_account_assets->account_id = admin_id;
    get_account_assets->asset_id = asset_id;
    get_account_assets->creator_account_id = admin_id;
    query = get_account_assets;

    accountAsset.asset_id = asset_id;
    accountAsset.account_id = admin_id;
    iroha::Amount amount(100, 2);
    accountAsset.balance = amount;

    role_permissions = {can_get_my_acc_ast};
  }
  std::shared_ptr<GetAccountAssets> get_account_assets;
  AccountAsset accountAsset;
};

/**
 * @given initialized storage, permission to his account
 * @when get account assets
 * @then Return account asset of user
 */
TEST_F(GetAccountAssetsTest, MyAccountValidCase) {
  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getAccountAsset(admin_id, asset_id))
      .WillOnce(Return(accountAsset));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountAssetResponse>(response);
  ASSERT_EQ(cast_resp->acct_asset.account_id, admin_id);
}

/**
 * @given initialized storage, global permission
 * @when get account information about other user
 * @then Return account asset
 */
TEST_F(GetAccountAssetsTest, AllAccountValidCase) {
  get_account_assets->account_id = account_id;
  accountAsset.account_id = account_id;
  role_permissions = {can_get_all_acc_ast};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getAccountAsset(account_id, asset_id))
      .WillOnce(Return(accountAsset));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountAssetResponse>(response);
  ASSERT_EQ(cast_resp->acct_asset.account_id, account_id);
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the same domain
 * @then Return account
 */
TEST_F(GetAccountAssetsTest, DomainAccountValidCase) {
  get_account_assets->account_id = account_id;
  accountAsset.account_id = account_id;
  role_permissions = {can_get_domain_acc_ast};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getAccountAsset(account_id, asset_id))
      .WillOnce(Return(accountAsset));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountAssetResponse>(response);
  ASSERT_EQ(cast_resp->acct_asset.account_id, account_id);
}

/**
 * @given initialized storage, granted permission
 * @when get account information about other user
 * @then Return error
 */
TEST_F(GetAccountAssetsTest, GrantAccountValidCase) {
  get_account_assets->account_id = account_id;
  accountAsset.account_id = account_id;
  role_permissions = {};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query,
              hasAccountGrantablePermission(
                  admin_id, get_account_assets->account_id, can_get_my_acc_ast))
      .WillOnce(Return(true));

  EXPECT_CALL(*wsv_query, getAccountAsset(account_id, asset_id))
      .WillOnce(Return(accountAsset));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::AccountAssetResponse>(response);
  ASSERT_EQ(cast_resp->acct_asset.account_id, account_id);
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the other domain
 * @then Return error
 */
TEST_F(GetAccountAssetsTest, DifferentDomainAccountInValidCase) {
  get_account_assets->account_id = "test@test2";
  accountAsset.account_id = account_id;
  role_permissions = {can_get_domain_acc_ast};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query,
              hasAccountGrantablePermission(
                  admin_id, get_account_assets->account_id, can_get_my_acc_ast))
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
TEST_F(GetAccountAssetsTest, NoAccountExist) {
  get_account_assets->account_id = "none";
  accountAsset.account_id = account_id;
  role_permissions = {can_get_all_acc_ast};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));

  EXPECT_CALL(*wsv_query,
              getAccountAsset(get_account_assets->account_id, asset_id))
      .WillOnce(Return(nonstd::nullopt));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::ErrorResponse>(response);
  ASSERT_EQ(cast_resp->reason, ErrorResponse::NO_ACCOUNT_ASSETS);
}

/// --------- Get Signatories-------------
class GetSignatoriesTest : public QueryValidateExecuteTest {
 public:
  void SetUp() override {
    QueryValidateExecuteTest::SetUp();
    get_signatories = std::make_shared<GetSignatories>();
    get_signatories->account_id = admin_id;
    get_signatories->creator_account_id = admin_id;
    query = get_signatories;
    signs = {iroha::pubkey_t()};
    role_permissions = {can_get_my_signatories};
  }
  std::shared_ptr<GetSignatories> get_signatories;
  std::vector<iroha::pubkey_t> signs;
};

/**
 * @given initialized storage, permission to his account
 * @when get account assets
 * @then Return account asset of user
 */
TEST_F(GetSignatoriesTest, MyAccountValidCase) {
  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getSignatories(admin_id)).WillOnce(Return(signs));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::SignatoriesResponse>(response);
  ASSERT_EQ(cast_resp->keys.size(), 1);
}

/**
 * @given initialized storage, global permission
 * @when get account information about other user
 * @then Return account asset
 */
TEST_F(GetSignatoriesTest, AllAccountValidCase) {
  get_signatories->account_id = account_id;
  role_permissions = {can_get_all_signatories};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getSignatories(account_id)).WillOnce(Return(signs));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::SignatoriesResponse>(response);
  ASSERT_EQ(cast_resp->keys.size(), 1);
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the same domain
 * @then Return account
 */
TEST_F(GetSignatoriesTest, DomainAccountValidCase) {
  get_signatories->account_id = account_id;
  role_permissions = {can_get_domain_signatories};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query, getSignatories(account_id)).WillOnce(Return(signs));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::SignatoriesResponse>(response);
  ASSERT_EQ(cast_resp->keys.size(), 1);
}

/**
 * @given initialized storage, granted permission
 * @when get account information about other user
 * @then Return error
 */
TEST_F(GetSignatoriesTest, GrantAccountValidCase) {
  get_signatories->account_id = account_id;
  role_permissions = {};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(
      *wsv_query,
      hasAccountGrantablePermission(
          admin_id, get_signatories->account_id, can_get_my_signatories))
      .WillOnce(Return(true));

  EXPECT_CALL(*wsv_query, getSignatories(account_id)).WillOnce(Return(signs));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::SignatoriesResponse>(response);
  ASSERT_EQ(cast_resp->keys.size(), 1);
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the other domain
 * @then Return error
 */
TEST_F(GetSignatoriesTest, DifferentDomainAccountInValidCase) {
  get_signatories->account_id = "test@test2";
  role_permissions = {can_get_domain_signatories};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(
      *wsv_query,
      hasAccountGrantablePermission(
          admin_id, get_signatories->account_id, can_get_my_signatories))
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
TEST_F(GetSignatoriesTest, NoAccountExist) {
  get_signatories->account_id = "none";
  role_permissions = {can_get_all_signatories};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));

  EXPECT_CALL(*wsv_query, getSignatories(get_signatories->account_id))
      .WillOnce(Return(nonstd::nullopt));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::ErrorResponse>(response);
  ASSERT_EQ(cast_resp->reason, ErrorResponse::NO_SIGNATORIES);
}

/// --------- Get Account Transactions-------------
class GetAccountTransactionsTest : public QueryValidateExecuteTest {
 public:
  void SetUp() override {
    QueryValidateExecuteTest::SetUp();
    get_tx = std::make_shared<GetAccountTransactions>();
    get_tx->account_id = admin_id;
    get_tx->creator_account_id = admin_id;
    query = get_tx;
    role_permissions = {can_get_my_acc_txs};
    N = 3;
    txs_observable = rxcpp::observable<>::iterate([this] {
      std::vector<iroha::model::Transaction> result;
      for (size_t i = 0; i < N; ++i) {
        iroha::model::Transaction current;
        current.creator_account_id = account_id;
        current.tx_counter = i;
        result.push_back(current);
      }
      return result;
    }());
  }
  std::shared_ptr<GetAccountTransactions> get_tx;
  rxcpp::observable<Transaction> txs_observable;
  size_t N;
};

/**
 * @given initialized storage, permission to his account
 * @when get account assets
 * @then Return account asset of user
 */
TEST_F(GetAccountTransactionsTest, MyAccountValidCase) {
  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));

  txs_observable = rxcpp::observable<>::iterate([this] {
    std::vector<iroha::model::Transaction> result;
    for (size_t i = 0; i < N; ++i) {
      iroha::model::Transaction current;
      current.creator_account_id = admin_id;
      current.tx_counter = i;
      result.push_back(current);
    }
    return result;
  }());

  EXPECT_CALL(*block_query, getAccountTransactions(admin_id))
      .WillOnce(Return(txs_observable));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::TransactionsResponse>(response);

  cast_resp->transactions.as_blocking().subscribe(
      [this](auto tx) { ASSERT_EQ(admin_id, tx.creator_account_id); }, []() {});
}

/**
 * @given initialized storage, global permission
 * @when get account information about other user
 * @then Return account asset
 */
TEST_F(GetAccountTransactionsTest, AllAccountValidCase) {
  get_tx->account_id = account_id;
  role_permissions = {can_get_all_acc_txs};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));

  EXPECT_CALL(*block_query, getAccountTransactions(account_id))
      .WillOnce(Return(txs_observable));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::TransactionsResponse>(response);
  cast_resp->transactions.as_blocking().subscribe(
      [this](auto tx) { ASSERT_EQ(account_id, tx.creator_account_id); },
      []() {});
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the same domain
 * @then Return account
 */
TEST_F(GetAccountTransactionsTest, DomainAccountValidCase) {
  get_tx->account_id = account_id;
  role_permissions = {can_get_domain_acc_txs};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));

  EXPECT_CALL(*block_query, getAccountTransactions(account_id))
      .WillOnce(Return(txs_observable));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::TransactionsResponse>(response);
  cast_resp->transactions.as_blocking().subscribe(
      [this](auto tx) { ASSERT_EQ(account_id, tx.creator_account_id); },
      []() {});
}

/**
 * @given initialized storage, granted permission
 * @when get account information about other user
 * @then Return error
 */
TEST_F(GetAccountTransactionsTest, GrantAccountValidCase) {
  get_tx->account_id = account_id;
  role_permissions = {};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query,
              hasAccountGrantablePermission(
                  admin_id, get_tx->account_id, can_get_my_acc_txs))
      .WillOnce(Return(true));

  EXPECT_CALL(*block_query, getAccountTransactions(account_id))
      .WillOnce(Return(txs_observable));
  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::TransactionsResponse>(response);
  cast_resp->transactions.as_blocking().subscribe(
      [this](auto tx) { ASSERT_EQ(account_id, tx.creator_account_id); },
      []() {});
}

/**
 * @given initialized storage, domain permission
 * @when get account information about other user in the other domain
 * @then Return error
 */
TEST_F(GetAccountTransactionsTest, DifferentDomainAccountInValidCase) {
  get_tx->account_id = "test@test2";
  role_permissions = {can_get_domain_acc_ast};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));
  EXPECT_CALL(*wsv_query,
              hasAccountGrantablePermission(
                  admin_id, get_tx->account_id, can_get_my_acc_txs))
      .WillOnce(Return(false));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::ErrorResponse>(response);
  ASSERT_EQ(cast_resp->reason, ErrorResponse::STATEFUL_INVALID);
}

/**
 * @given initialized storage, permission
 * @when get account information about non existing account
 * @then Return empty response
 */
TEST_F(GetAccountTransactionsTest, NoAccountExist) {
  get_tx->account_id = "none";
  role_permissions = {can_get_all_acc_txs};

  EXPECT_CALL(*wsv_query, getAccountRoles(admin_id))
      .WillOnce(Return(admin_roles));
  EXPECT_CALL(*wsv_query, getRolePermissions(admin_role))
      .WillOnce(Return(role_permissions));

  EXPECT_CALL(*block_query, getAccountTransactions(get_tx->account_id))
      .WillOnce(Return(rxcpp::observable<>::empty<Transaction>()));

  auto response = validateAndExecute();
  auto cast_resp =
      std::static_pointer_cast<iroha::model::TransactionsResponse>(response);
}