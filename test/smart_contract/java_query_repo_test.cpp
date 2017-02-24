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
#include <repository/domain/account_repository.hpp>
#include <repository/domain/asset_repository.hpp>
#include <repository/world_state_repository.hpp>
#include <virtual_machine/virtual_machine.hpp>
#include <infra/virtual_machine/jvm/java_data_structure.hpp>

const std::string PackageName = "test";
const std::string ContractName = "Test";
const std::string PublicKeyTag = "publicKey";
const std::string DomainIdTag = "domainId";
const std::string AccountNameTag = "accountName";
const std::string AssetNameTag = "assetName";
const std::string AssetValueTag = "assetValue";
const std::string SmartContractNameTag = "smartContractName";

TEST(JavaQueryRepo, InitializeVM) {
  virtual_machine::initializeVM(PackageName, ContractName);
}

TEST(JavaQueryRepo, Invoke_JAVA_function) {
  const std::string FunctionName = "test1";
  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName);
}

TEST(JavaQueryRepo, Invoke_JAVA_function_map_argv) {

  const std::string FunctionName = "test2";

  std::map<std::string, std::string> params;
  {
    params["key1"] = "Mizuki";
    params["key2"] = "Sonoko";
  }

  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName,
                                  params);
}

TEST(JavaQueryRepo, Invoke_JAVA_function_map_utf_8) {

  const std::string FunctionName = "test3";

  std::map<std::string, std::string> params;
  {
    params["key1"] = "水樹";
    params["key2"] = "素子";
  }

  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName,
                                  params);
}

TEST(JavaQueryRepo, Invoke_CPP_account_repo_function_FROM_JAVA_function) {

  const std::string FunctionName = "testAddAccount";

  std::map<std::string, std::string> params;
  {
    params[PublicKeyTag] = "MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=";
    params[AccountNameTag] = "MizukiSonoko";
  }

  std::vector<std::string> assets;
  {
    assets.push_back("asset1");
    assets.push_back("asset2");
    assets.push_back("asset3");
  }

  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName,
                                  params, assets);

  const std::string uuid =
      "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";

  const std::string received_serialized_acc =
      repository::world_state_repository::find(uuid);

  Api::Account account;
  account.ParseFromString(received_serialized_acc);

  ASSERT_STREQ(params[PublicKeyTag].c_str(), account.publickey().c_str());
  ASSERT_STREQ(params[AccountNameTag].c_str(), account.name().c_str());
  for (std::size_t i = 0; i < assets.size(); i++) {
    ASSERT_STREQ(assets[i].c_str(), account.assets(i).c_str());
  }

  // If consle shows "Failed" but gTest passes, try remove cache of leveldb
  // (maybe "/tmp/iroha_ledger") then run test again.
}

TEST(JavaQueryRepo, Invoke_CPP_asset_repo_function_FROM_JAVA_function) {

  const std::string FunctionName = "testAddAsset";

  std::map<std::string, std::string> assetInfo;
  {
    assetInfo[DomainIdTag] = "A domain id";
    assetInfo[AssetNameTag] = "Currency";
    assetInfo[SmartContractNameTag] = "smartContractFunc";
  }

  std::map<std::string, std::map<std::string, std::string>> assetValue;
  {
    assetValue["ownerName"] = {
        {"type", "string"},
        {"value", "karin"}
    };

    assetValue["my_money"] = {
        {"type", "int"},
        {"value", "123456"}
    };

    assetValue["rate"] = {
        {"type", "double"},
        {"value", "0.12345678901234567890123456789"}
    };
  }

  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName,
                                  assetInfo, assetValue);
 
  const std::string uuid =
      "3f8ba1e5df7f1587defc8fae4789207c8719c7b6d86ce299821b8a83fe08b5a9";

  // TODO: Getting uuid needs to be able to invoke non-void method.
  const std::string received_asset_value =
      repository::world_state_repository::find(uuid);

  using virtual_machine::jvm::convertBaseObjectToMapString;

  Api::Asset asset;
  asset.ParseFromString(received_asset_value);

  ASSERT_STREQ(assetInfo[DomainIdTag].c_str(), asset.domain().c_str());
  ASSERT_STREQ(assetInfo[AssetNameTag].c_str(), asset.name().c_str());
  for (auto &&e: assetValue) {
    std::map<std::string, std::string> lhs = e.second;
    Api::BaseObject rhsObj = asset.value().find(e.first)->second;
    std::map<std::string, std::string> rhs = convertBaseObjectToMapString(rhsObj);

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    std::cout << "(" << lhs["type"] << ", " << lhs["value"] << ") vs "
              << "(" << rhs["type"] << ", " << rhs["value"] << ")\n";

    if (lhs["type"] != rhs["type"]) {
      throw "Mismatch type";
    }

    if (lhs["type"] == "double") {
      const auto lhsValue = std::stod(lhs["value"]);
      const auto rhsValue = std::stod(rhs["value"]);
      ASSERT_TRUE(abs(lhsValue - rhsValue) < 1e-5);
    } else {
      ASSERT_TRUE(lhs["value"] == rhs["value"]);
    }

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
  }

  for (auto &&e: asset.value()) {
    std::map<std::string, std::string> lhs = assetValue.find(e.first)->second;
    Api::BaseObject rhsObj = e.second;
    std::map<std::string, std::string> rhs = convertBaseObjectToMapString(rhsObj);
    
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    std::cout << "(" << lhs["type"] << ", " << lhs["value"] << ") vs "
              << "(" << rhs["type"] << ", " << rhs["value"] << ")\n";

    if (lhs["type"] != rhs["type"]) {
      throw "Mismatch type";
    }

    if (lhs["type"] == "double") {
      const auto lhsValue = std::stod(lhs["value"]);
      const auto rhsValue = std::stod(rhs["value"]);
      ASSERT_TRUE(abs(lhsValue - rhsValue) < 1e-5);
    } else {
      ASSERT_TRUE(lhs["value"] == rhs["value"]);
    }

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
  }

  ASSERT_STREQ(assetInfo[SmartContractNameTag].c_str(), asset.smartcontractname().c_str());
}

TEST(JavaQueryRepo, FinishVM) {
  virtual_machine::finishVM(PackageName, ContractName);
}
