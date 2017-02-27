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
#include <repository/domain/asset_repository.hpp>
#include <repository/world_state_repository.hpp>
#include <virtual_machine/virtual_machine.hpp>
#include <infra/virtual_machine/jvm/java_data_structure.hpp>

const std::string PackageName = "test";
const std::string ContractName = "Test";

const std::string PublicKeyTag = "publicKey";
const std::string DomainIdTag = "domainId";
const std::string AssetNameTag = "assetName";
const std::string AssetValueTag = "assetValue";
const std::string SmartContractNameTag = "smartContractName";

std::map<std::string, std::string> assetInfo;
std::map<std::string, std::map<std::string, std::string>> assetValue;

const auto assetUuid = "3f8ba1e5df7f1587defc8fae4789207c8719c7b6d86ce299821b8a83fe08b5a9";

void ensureIntegrityOfAsset() {
  const std::string received_asset_value =
      repository::world_state_repository::find(assetUuid);

  // Restore asset.
  Api::Asset asset;
  asset.ParseFromString(received_asset_value);

  ASSERT_STREQ(assetInfo[DomainIdTag].c_str(), asset.domain().c_str());
  ASSERT_STREQ(assetInfo[AssetNameTag].c_str(), asset.name().c_str());
  ASSERT_STREQ(assetInfo[SmartContractNameTag].c_str(), asset.smartcontractname().c_str());

  using virtual_machine::jvm::convertBaseObjectToMapString;

  for (auto &&e: assetValue) {
    std::map<std::string, std::string> lhs = e.second;
    Api::BaseObject rhsObj = asset.value().find(e.first)->second;
    std::map<std::string, std::string> rhs = convertBaseObjectToMapString(rhsObj);

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
  }

  for (auto &&e: asset.value()) {
    std::map<std::string, std::string> lhs = assetValue.find(e.first)->second;
    Api::BaseObject rhsObj = e.second;
    std::map<std::string, std::string> rhs = convertBaseObjectToMapString(rhsObj);
    
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
  }
}

/*********************************************************************************************************
 * Test Asset
 *********************************************************************************************************/
TEST(JavaQueryRepoAsset, InitializeVM) {
  virtual_machine::initializeVM(PackageName, ContractName);
}

TEST(JavaQueryRepoAsset, removeDBChacheIfExists) {
  if (repository::asset::exists(assetUuid)) {
    repository::asset::remove(assetUuid);
  }
}

TEST(JavaQueryRepoAsset, invokeAddAssetQuery) {

  /***********************************************************************
   * 1. Initial guess
   ***********************************************************************/
  assetInfo[DomainIdTag] = "A domain id";
  assetInfo[AssetNameTag] = "Currency";
  assetInfo[SmartContractNameTag] = "smartContractFunc";

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

  /***********************************************************************
   * 2. Invoke Java
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName, "testAddAsset",
                                  assetInfo, assetValue);

  /***********************************************************************
   * 3. Test
   ***********************************************************************/
  std::cout << "In c++:\n";
  ensureIntegrityOfAsset();
}

TEST(JavaQueryRepoAsset, invokeUpdateAssetQuery) {
  /***********************************************************************
   * 1. Initial guess
   ***********************************************************************/
  assetValue["ownerName"] = {
      {"type", "string"},
      {"value", "asuka"}
  };

  assetValue["my_money"] = {
      {"type", "int"},
      {"value", "98765"}
  };

  assetValue["updated_value"] = {
      {"type", "string"},
      {"value", "アセット更新のテスト"}
  };

  /***********************************************************************
   * 2. Invocation Java.
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName, "testUpdateAsset",
                                  assetUuid, assetValue);

  /***********************************************************************
   * 3. Test
   ***********************************************************************/
  std::cout << "In c++:\n";
  ensureIntegrityOfAsset();

}

TEST(JavaQueryRepoAsset, invokeRemoveAssetQuery) {
  /***********************************************************************
   * 1. Invocation Java.
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName, "testRemoveAsset",
                                  assetUuid);
  /***********************************************************************
   * 2. Test
   ***********************************************************************/
  ASSERT_FALSE(repository::asset::exists(assetUuid));

}

TEST(JavaQueryRepoAsset, reinvokeAddAssetQuery) {
  /***********************************************************************
   * 1. Invocation Java.
   ***********************************************************************/
  assetInfo[DomainIdTag] = "アナザーDOMAIN";
  assetInfo[AssetNameTag] = "ポイント";
  assetInfo[SmartContractNameTag] = "anotherSmartContractFunc";
  virtual_machine::invokeFunction(PackageName, ContractName, "testAddAsset",
                                  assetInfo, assetValue);
  /***********************************************************************
   * 2. Test
   ***********************************************************************/
  const auto newAssetUuid = "48578a1dd980bc7b739702889057f292f3cb29f7a67307fbce04f2e34489eb57";
  ASSERT_FALSE(repository::asset::exists(assetUuid));
  ASSERT_TRUE(repository::asset::exists(newAssetUuid));

}

TEST(JavaQueryRepoAsset, FinishVM) {
  virtual_machine::finishVM(PackageName, ContractName);
}
