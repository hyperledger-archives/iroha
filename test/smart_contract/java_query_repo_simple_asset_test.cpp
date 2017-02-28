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
#include <infra/virtual_machine/jvm/java_data_structure.hpp>
#include <repository/domain/simple_asset_repository.hpp>
#include <repository/world_state_repository.hpp>
#include <transaction_builder/helper/create_objects_helper.hpp>
#include <virtual_machine/virtual_machine.hpp>

const std::string PackageName = "test";
const std::string ContractName = "TestSimpleAsset";

const std::string DomainIdTag = "domainId";
const std::string SimpleAssetNameTag = "simpleAssetName";
const std::string SimpleAssetValueTag = "simpleAssetValue";
const std::string SmartContractNameTag = "smartContractName";

void ensureIntegrityOfSimpleAssetValue(
    std::string uuid, std::map<std::string, std::string> inputInfo,
    std::map<std::string, std::string> inputValue) {

  /*******************************************************************************
   * Restore SimpleAsset from DB
   *******************************************************************************/
  const std::string strValue = repository::world_state_repository::find(uuid);

  Api::SimpleAsset simpleAsset;
  { simpleAsset.ParseFromString(strValue); }

  ASSERT_STREQ(inputInfo[DomainIdTag].c_str(), simpleAsset.domain().c_str());
  ASSERT_STREQ(inputInfo[SimpleAssetNameTag].c_str(),
               simpleAsset.name().c_str());
  ASSERT_STREQ(inputInfo[SmartContractNameTag].c_str(),
               simpleAsset.smartcontractname().c_str());

  using virtual_machine::jvm::convertBaseObjectToMapString;

  auto outputValue = convertBaseObjectToMapString(simpleAsset.value());

  if (inputValue["type"] == "double") {
    const auto lhsValue = std::stod(inputValue["value"]);
    const auto rhsValue = std::stod(outputValue["value"]);
    std::cout << lhsValue << " vs " << rhsValue << std::endl;
    std::cout << "Warning: Double error is " << abs(lhsValue - rhsValue)
              << std::endl;
    ASSERT_TRUE(abs(lhsValue - rhsValue) < 1e-5);
  } else {
    std::cout << inputValue["value"] << " vs " << outputValue["value"]
              << std::endl;
    ASSERT_TRUE(inputValue["value"] == outputValue["value"]);
  }
}

/*********************************************************************************************************
 * Test SimpleAsset
 *********************************************************************************************************/
TEST(JavaQueryRepoSimpleAsset, initializeVM) {
  virtual_machine::initializeVM(PackageName, ContractName);
}

TEST(JavaQueryRepoSimpleAsset, invokeAddSimpleAssetQuery) {

  /***********************************************************************
   * 1. Initialize
   ***********************************************************************/
  // Remove cache
  const auto uuid =
      "6adb762d04a3744f32ec2027e820efeb543c79cd86e9b0cd51c8469cd25a453e";
  if (repository::simple_asset::exists(uuid)) {
    ASSERT_TRUE(repository::simple_asset::remove(uuid));
  }

  std::map<std::string, std::string> inputInfo;
  {
    inputInfo[DomainIdTag] = "domain id";
    inputInfo[SimpleAssetNameTag] = "simple asset";
    inputInfo[SmartContractNameTag] = "smartContractFunc";
  }

  std::map<std::string, std::string> inputValue;
  {
    inputValue["type"] = "double";
    inputValue["value"] = "98765.432101234567890987654321";
  }

  /***********************************************************************
   * 2. Invoke Java
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName,
                                  "testAddSimpleAsset", inputInfo, inputValue);

  /***********************************************************************
   * 3. Test
   ***********************************************************************/
  ensureIntegrityOfSimpleAssetValue(uuid, inputInfo, inputValue);

  // Remove chache again.
  ASSERT_TRUE(repository::simple_asset::remove(uuid));
}

TEST(JavaQueryRepoSimpleAsset, invokeUpdateSimpleAssetQuery) {

  /***********************************************************************
   * 1. Initialize
   ***********************************************************************/
  std::map<std::string, std::string> inputInfo;
  {
    inputInfo[DomainIdTag] = "this is domain id";
    inputInfo[SimpleAssetNameTag] = "this is simple asset name";
    inputInfo[SmartContractNameTag] = "this is smart contract tag";
  }

  const auto uuid = repository::simple_asset::add(
      inputInfo[DomainIdTag], inputInfo[SimpleAssetNameTag],
      txbuilder::createValueBool(true), inputInfo[SmartContractNameTag]);

  ASSERT_FALSE(uuid.empty());

  std::map<std::string, std::string> inputUpdatedValue;
  {
    inputUpdatedValue["type"] = "string";
    inputUpdatedValue["value"] = "updated from double to string";
  }

  /***********************************************************************
   * 2. Invocation Java.
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName,
                                  "testUpdateSimpleAsset", uuid,
                                  inputUpdatedValue);

  /***********************************************************************
   * 3. Test
   ***********************************************************************/
  const auto simpleAsset = repository::simple_asset::findByUuid(uuid);

  std::map<std::string, std::string> info;
  // info doesn't change.
  info.emplace(DomainIdTag, simpleAsset.domain());
  info.emplace(SimpleAssetNameTag, simpleAsset.name());
  info.emplace(SmartContractNameTag, simpleAsset.smartcontractname());

  ensureIntegrityOfSimpleAssetValue(uuid, info, inputUpdatedValue);

  // Remove chache again.
  ASSERT_TRUE(repository::simple_asset::remove(uuid));
}

TEST(JavaQueryRepoSimpleAsset, invokeRemoveSimpleAssetQuery) {

  auto uuid = repository::simple_asset::add("domain id", "simple asset",
                                            txbuilder::createValueString("smpl value"), "");
  ASSERT_TRUE(repository::simple_asset::exists(uuid));

  /***********************************************************************
   * 1. Invocation Java.
   ***********************************************************************/
  virtual_machine::invokeFunction(PackageName, ContractName,
                                  "testRemoveSimpleAsset", uuid);
  /***********************************************************************
   * 2. Test
   ***********************************************************************/
  ASSERT_FALSE(repository::simple_asset::exists(uuid));
  ASSERT_FALSE(repository::simple_asset::remove(uuid));
}

TEST(JavaQueryRepoSimpleAsset, finishVM) {
  virtual_machine::finishVM(PackageName, ContractName);
}
