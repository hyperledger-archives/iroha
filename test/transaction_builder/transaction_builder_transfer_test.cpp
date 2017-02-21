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
using type_signatures::Transfer;

/***************************************************************************
  Transfer
 ***************************************************************************/
TEST(transaction_builder, create_transfer) {

  {
    auto txDomain = TransactionBuilder<Transfer<object::Domain>>()
      .setSender("karin")
      .setReceiverPublicKey("receiverPublicKey")
      .setDomain(object::Domain({"pubkey1", "pubkey2"}, "name"))
      .build();

    ASSERT_TRUE(txDomain.senderPublicKey == "karin");
    auto addc = txDomain.command.AsTransfer();
    auto domo = addc->object.AsDomain();
    ASSERT_TRUE(domo->ownerPublicKey == std::vector<std::string>({"pubkey1", "pubkey2"}));
    ASSERT_TRUE(domo->name == "name");
  }

  {
    auto txAccount = TransactionBuilder<Transfer<object::Account>>()
      .setSender("karin")
      .setReceiverPublicKey("receiverPublicKey")
      .setAccount(object::Account("pubkey", "name"))
      .build();

    ASSERT_TRUE(txAccount.senderPublicKey == "karin");
    auto addc = txAccount.command.AsTransfer();
    auto acco = addc->object.AsAccount();
    ASSERT_TRUE(acco->ownerPublicKey == "pubkey");
    ASSERT_TRUE(acco->name == "name");
  }

  {
    auto value = std::unordered_map<std::string, object::BaseObject>(
      {{"key1", object::BaseObject("value")}, {"key2", object::BaseObject(123456)}}
    );
    auto txAsset = TransactionBuilder<Transfer<object::Asset>>()
      .setSender("karin")
      .setReceiverPublicKey("receiverPublicKey")
      .setAsset(object::Asset("domainID", "name", value))
      .build();

    ASSERT_TRUE(txAsset.senderPublicKey == "karin");
    auto addc = txAsset.command.AsTransfer();
    auto asso = addc->object.AsAsset();
    ASSERT_TRUE(asso->domain == "domainID");
    ASSERT_TRUE(asso->name == "name");
    ASSERT_TRUE(asso->value == value);
  }
}

