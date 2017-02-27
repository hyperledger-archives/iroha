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
#include <repository/domain/simple_asset_repository.hpp>
#include <repository/world_state_repository.hpp>
#include <virtual_machine/virtual_machine.hpp>
#include <infra/virtual_machine/jvm/java_data_structure.hpp>

const std::string PackageName = "test";
const std::string ContractName = "TestSimpleAsset";

const std::string DomainIdTag = "domainId";
const std::string SimpleAssetNameTag = "simpleAssetName";
const std::string SimpleAssetValueTag = "simpleAssetValue";
const std::string SmartContractNameTag = "smartContractName";

const std::string uuid =
    "6adb762d04a3744f32ec2027e820efeb543c79cd86e9b0cd51c8469cd25a453e";

std::map<std::string, std::string> inputInfo;
std::map<std::string, std::string> inputValue;

void ensureIntegrityOfSimpleAssetValue() {

  /*******************************************************************************
   * Restore SimpleAsset from DB
   *******************************************************************************/
  const std::string strValue =
      repository::world_state_repository::find(uuid);

  Api::SimpleAsset simpleAsset;
  {
    simpleAsset.ParseFromString(strValue);
  }

  ASSERT_STREQ(inputInfo[DomainIdTag].c_str(),           simpleAsset.domain().c_str());
  ASSERT_STREQ(inputInfo[SimpleAssetNameTag].c_str(),    simpleAsset.name().c_str());
  ASSERT_STREQ(inputInfo[SmartContractNameTag].c_str(),  simpleAsset.smartcontractname().c_str());

  using virtual_machine::jvm::convertBaseObjectToMapString;

  auto outputValue = convertBaseObjectToMapString(simpleAsset.value());

  if (inputValue["type"] == "double") {
    const auto lhsValue = std::stod(inputValue["value"]);
    const auto rhsValue = std::stod(outputValue["value"]);
    std::cout << lhsValue << " vs " << rhsValue << std::endl;
    std::cout << "Warning: Double error is " << abs(lhsValue - rhsValue) << std::endl;
    ASSERT_TRUE(abs(lhsValue - rhsValue) < 1e-5);
  } else {
    std::cout << inputValue["value"] << " vs " << outputValue["value"] << std::endl;
    ASSERT_TRUE(inputValue["value"] == outputValue["value"]);
  }

}

/*********************************************************************************************************
 * Test SimpleAsset
 *********************************************************************************************************/
TEST(JavaQueryRepoSimpleAsset, InitializeVM) {
  virtual_machine::initializeVM(PackageName, ContractName);
}

/***********************************************************************
 * Remove cache
 ***********************************************************************/
TEST(JavaQueryRepoSimpleAsset, removeDBChacheIfExists) {
  const auto account = "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";
  if (repository::simple_asset::exists(account)) {
    repository::simple_asset::remove(account);
  }
  const auto account2 = "48578a1dd980bc7b739702889057f292f3cb29f7a67307fbce04f2e34489eb57";
  if (repository::simple_asset::exists(account2)) {
    repository::simple_asset::remove(account2);
  }
}

TEST(JavaQueryRepoSimpleAsset, invokeAddSimpleAssetQuery) {
  /***********************************************************************
   * 1. Initial guess
   ***********************************************************************/
  
  inputInfo[DomainIdTag]           = "domain id";
  inputInfo[SimpleAssetNameTag]    = "simple asset";
  inputInfo[SmartContractNameTag]  = "smartContractFunc";

  inputValue["type"]  = "double";
  inputValue["value"] = "98765.432101234567890987654321";

  /***********************************************************************
   * 2. Invoke Java
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName, "testAddSimpleAsset",
                                  inputInfo, inputValue);

  /***********************************************************************
   * 3. Test
   ***********************************************************************/
  ensureIntegrityOfSimpleAssetValue();
}

TEST(JavaQueryRepoSimpleAsset, invokeUpdateSimpleAssetQuery) {
  /***********************************************************************
   * 1. Initial guess
   ***********************************************************************/
  inputValue["type"] = "string";
  inputValue["value"] = "updated from double to string";

  /***********************************************************************
   * 2. Invocation Java.
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName, "testUpdateSimpleAsset",
                                  uuid, inputValue);

  /***********************************************************************
   * 3. Test
   ***********************************************************************/
  std::cout << "In c++:\n";
  ensureIntegrityOfSimpleAssetValue();

}

TEST(JavaQueryRepoSimpleAsset, invokeRemoveSimpleAssetQuery) {
  /***********************************************************************
   * 1. Invocation Java.
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName, "testRemoveSimpleAsset",
                                  uuid);
  /***********************************************************************
   * 2. Test
   ***********************************************************************/
  ASSERT_FALSE(repository::simple_asset::exists(uuid));

}

TEST(JavaQueryRepoSimpleAsset, FinishVM) {
  virtual_machine::finishVM(PackageName, ContractName);
}
