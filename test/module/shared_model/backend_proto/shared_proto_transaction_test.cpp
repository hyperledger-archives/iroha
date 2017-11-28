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

#include "backend/protobuf/transaction.hpp"
#include "builders/protobuf/proto_transaction_builder.hpp"

#include <gtest/gtest.h>

/**
 * @given protobuf transaction with transaction counter set
 * @when converted to shared model
 * @then shared model is created correctly
 */
TEST(ProtoTransaction, Create) {
  iroha::protocol::Transaction transaction;
  transaction.mutable_payload()->set_tx_counter(1);
  shared_model::proto::Transaction proto(transaction);
  ASSERT_EQ(proto.transactionCounter(), transaction.payload().tx_counter());
}

/**
 * @given transaction field values and sample command values
 * @when create transaction with sample command using transaction builder
 * @then transaction is built correctly
 */
TEST(ProtoTransaction, Builder) {
  shared_model::interface::Transaction::TxCounterType tx_counter = 1;
  std::string account_id = "admin@test", asset_id = "coin#test",
              amount = "10.00";
  auto tx = shared_model::proto::TransactionBuilder()
                .txCounter(tx_counter)
                .creatorAccountId(account_id)
                .addAssetQuantity(account_id, asset_id, amount)
                .build();
  ASSERT_EQ(account_id, tx.creatorAccountId());
  ASSERT_EQ(tx_counter, tx.transactionCounter());
  ASSERT_EQ(1, tx.commands().size());
  auto &aaq = boost::
      get<shared_model::detail::
              PolymorphicWrapper<shared_model::interface::AddAssetQuantity>>(
          tx.commands().at(0)->get());
  ASSERT_EQ(aaq->accountId(), account_id);
  ASSERT_EQ(aaq->assetId(), asset_id);
  ASSERT_EQ(aaq->amount().intValue(), 1000);
  ASSERT_EQ(aaq->amount().precision(), 2);
}
