/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <gtest/gtest.h>
#include <transaction_builder/transaction_builder.hpp>
#include <util/exception.hpp>

using transaction::TransactionBuilder;
using type_signatures::Remove;

/***************************************************************************
  Remove
 ***************************************************************************/
TEST(transaction_builder, create_remove) {

  {
    auto txDomain = TransactionBuilder<Remove<object::Domain>>()
      .setSender("karin")
      .setDomain(object::Domain({"pubkey1", "pubkey2"}, "name"))
      .build();

    ASSERT_TRUE(txDomain.senderPublicKey == "karin");
    auto addc = txDomain.command.AsRemove();
    auto domo = addc->object.AsDomain();
    ASSERT_TRUE(domo->ownerPublicKey == std::vector<std::string>({"pubkey1", "pubkey2"}));
    ASSERT_TRUE(domo->name == "name");
  }

  {
    auto txAccount = TransactionBuilder<Remove<object::Account>>()
      .setSender("karin")
      .setAccount(object::Account("pubkey", "name"))
      .build();

    ASSERT_TRUE(txAccount.senderPublicKey == "karin");
    auto addc = txAccount.command.AsRemove();
    auto acco = addc->object.AsAccount();
    ASSERT_TRUE(acco->ownerPublicKey == "pubkey");
    ASSERT_TRUE(acco->name == "name");
  }

  {
    auto value = std::unordered_map<std::string, object::BaseObject>(
      {{"key1", object::BaseObject("value")}, {"key2", object::BaseObject(123456)}}
    );
    auto txAsset = TransactionBuilder<Remove<object::Asset>>()
      .setSender("karin")
      .setAsset(object::Asset("domainID", "name", value))
      .build();

    ASSERT_TRUE(txAsset.senderPublicKey == "karin");
    auto addc = txAsset.command.AsRemove();
    auto asso = addc->object.AsAsset();
    ASSERT_TRUE(asso->domain == "domainID");
    ASSERT_TRUE(asso->name == "name");
    ASSERT_TRUE(asso->value == value);
  }
}