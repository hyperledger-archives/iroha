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
#include <infra/protobuf/api.pb.h>
#include <transaction_builder/transaction_builder.hpp>
#include <util/exception.hpp>

using transaction::TransactionBuilder;
using type_signatures::Update;
using type_signatures::Domain;
using type_signatures::Account;
using type_signatures::Asset;
using type_signatures::SimpleAsset;
using type_signatures::Peer;

/***************************************************************************
  Update
 ***************************************************************************/
TEST(transaction_builder, create_update_domain) {
  Api::Domain domain;
  domain.set_ownerpublickey("pubkey1");
  domain.set_name("name");
  auto txDomain = TransactionBuilder<Update<Domain>>()
    .setSenderPublicKey("karin")
    .setDomain(domain)
    .build();

  ASSERT_STREQ(txDomain.senderpubkey().c_str(), "karin");
  ASSERT_STREQ(txDomain.type().c_str(), "Update");

  auto obj = txDomain.domain();
  ASSERT_STREQ(obj.ownerpublickey().c_str(), "pubkey1");
  ASSERT_STREQ(obj.name().c_str(), "name");
}

TEST(transaction_builder, create_update_account) {

  Api::Account account;
  {
    account.set_publickey("pubkey1");
    account.set_name("name");
  }

  std::vector<std::string> assets = {
    "asset1",
    "asset2"
  };
  for (auto&& e: assets) {
    account.add_assets(e);
  }
  auto txAccount = TransactionBuilder<Update<Account>>()
    .setSenderPublicKey("karin")
    .setAccount(account)
    .build();

  ASSERT_STREQ(txAccount.senderpubkey().c_str(), "karin");
  ASSERT_STREQ(txAccount.type().c_str(), "Update");

  auto obj = txAccount.account();
  ASSERT_STREQ(obj.publickey().c_str(), "pubkey1");
  ASSERT_STREQ(obj.name().c_str(), "name");
  for (int i = 0; i < obj.assets_size(); i++) {
    ASSERT_STREQ(obj.assets(i).c_str(), assets[i].c_str());
  }
}

TEST(transaction_builder, create_update_asset) {
  std::unordered_map<std::string, Api::BaseObject> value;
  { 
    {
      Api::BaseObject baseObject;
      baseObject.set_valuestring("value1");
      value.emplace("key1", baseObject);
    }
    {
      Api::BaseObject baseObject;
      baseObject.set_valueint(123456);
      value.emplace("key2", baseObject);
    }
  }

  // Needs create helper function?
  Api::Asset asset;
  {
    asset.set_domain("domainID");
    asset.set_name("name");
    google::protobuf::Map<std::string, Api::BaseObject> protoValue(value.begin(), value.end());
    *asset.mutable_value() = protoValue;
    asset.set_smartcontractname("contract_func");
  }

  auto txAsset = TransactionBuilder<Update<Asset>>()
    .setSenderPublicKey("karin")
    .setAsset(asset)
    .build();

  // Verify
  ASSERT_STREQ(txAsset.senderpubkey().c_str(), "karin");
  ASSERT_STREQ(txAsset.type().c_str(), "Update");

  auto obj = txAsset.asset();
  ASSERT_STREQ(obj.domain().c_str(), "domainID");
  ASSERT_STREQ(obj.name().c_str(), "name");

  ASSERT_TRUE(obj.value().find("key1")->second.value_case() == Api::BaseObject::ValueCase::kValueString);
  ASSERT_TRUE(obj.value().find("key2")->second.value_case() == Api::BaseObject::ValueCase::kValueInt);
  ASSERT_STREQ(obj.value().find("key1")->second.valuestring().c_str(), "value1");
  ASSERT_TRUE(obj.value().find("key2")->second.valueint() == 123456);
  ASSERT_STREQ(obj.smartcontractname().c_str(), "contract_func");
}

TEST(transaction_builder, create_update_peer) {

  Api::Trust trust;
  {
    trust.set_value(1.23);
  }

  Api::Peer peer;
  {
    peer.set_publickey("publickey");
    peer.set_address("address");
    *peer.mutable_trust() = trust;
  }

  auto txPeer = TransactionBuilder<Update<Peer>>()
    .setSenderPublicKey("karin")
    .setPeer(peer)
    .build();

  // Verify
  ASSERT_STREQ(txPeer.senderpubkey().c_str(), "karin");
  ASSERT_STREQ(txPeer.type().c_str(), "Update");

  auto obj = txPeer.peer();
  ASSERT_STREQ(obj.publickey().c_str(), "publickey");
  ASSERT_STREQ(obj.address().c_str(), "address");
  ASSERT_TRUE(obj.trust().value() == 1.23);
}

TEST(transaction_builder, create_update_simpleasset) {

  Api::BaseObject baseObject;
  {
    baseObject.set_valueint(123456);
  }

  Api::SimpleAsset simpleAsset;
  {
    simpleAsset.set_domain("domainID");
    simpleAsset.set_name("name");
    *simpleAsset.mutable_value() = baseObject;
    simpleAsset.set_smartcontractname("contract_func");
  }

  auto txSimpleAsset = TransactionBuilder<Update<SimpleAsset>>()
    .setSenderPublicKey("karin")
    .setSimpleAsset(simpleAsset)
    .build();

  // Verify
  ASSERT_STREQ(txSimpleAsset.senderpubkey().c_str(), "karin");
  ASSERT_STREQ(txSimpleAsset.type().c_str(), "Update");

  auto obj = txSimpleAsset.simpleasset();
  ASSERT_STREQ(obj.domain().c_str(), "domainID");
  ASSERT_STREQ(obj.name().c_str(), "name");

  ASSERT_TRUE(obj.value().value_case() == Api::BaseObject::ValueCase::kValueInt);
  ASSERT_TRUE(obj.value().valueint() == 123456);
  ASSERT_STREQ(obj.smartcontractname().c_str(), "contract_func");
}
