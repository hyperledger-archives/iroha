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

#include <../smart_contract/repository/jni_constants.hpp>
#include <infra/protobuf/api.pb.h>
#include <infra/virtual_machine/jvm/java_data_structure.hpp>
#include <repository/domain/account_repository.hpp>
#include <repository/world_state_repository.hpp>
#include <virtual_machine/virtual_machine.hpp>

const std::string PackageName = "test";
const std::string ContractName = "TestAccount";

namespace tag = jni_constants;

/*********************************************************************************************************
 * Test Account
 *********************************************************************************************************/
TEST(JavaQueryRepoAccount, initializeVM) {
  virtual_machine::initializeVM(PackageName, ContractName);
}

TEST(JavaQueryRepoAccount, invokeAddAccount) {

  /*****************************************************************
   * Remove chache
   *****************************************************************/
  const auto uuid =
      "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";

  if (repository::account::exists(uuid)) {
    repository::account::remove(uuid);
  }

  /*****************************************************************
   * Invoke Java method
   *****************************************************************/
  const std::string FunctionName = "testAddAccount";

  std::map<std::string, std::string> params;
  {
    params[tag::PublicKey]   = "MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=";
    params[tag::AccountName] = "MizukiSonoko";
  }

  std::vector<std::string> assets;
  {
    assets.push_back("asset1");
    assets.push_back("asset2");
    assets.push_back("asset3");
  }

  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName,
                                  params, assets);

  const std::string received_serialized_acc =
      repository::world_state_repository::find(uuid);

  Api::Account account;
  account.ParseFromString(received_serialized_acc);

  ASSERT_STREQ(params[tag::PublicKey].c_str(), account.publickey().c_str());
  ASSERT_STREQ(params[tag::AccountName].c_str(), account.name().c_str());
  for (std::size_t i = 0; i < assets.size(); i++) {
    ASSERT_STREQ(assets[i].c_str(), account.assets(i).c_str());
  }

  // Remove cache again
  ASSERT_TRUE(repository::account::remove(uuid));
}

TEST(JavaQueryRepoAccount, invokeAttachAssetToAccount) {

  /*****************************************************************
   * Remove chache
   *****************************************************************/
  const auto uuid =
      "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";

  if (repository::account::exists(uuid)) {
    repository::account::remove(uuid);
  }

  /*****************************************************************
   * Invoke Java method
   *****************************************************************/
  std::map<std::string, std::string> params;
  {
    params[tag::PublicKey]   = "MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=";
    params[tag::AccountName] = "MizukiSonoko";
  }

  std::vector<std::string> assets;
  {
    assets.push_back("asset1");
    assets.push_back("asset2");
    assets.push_back("asset3");
  }

  virtual_machine::invokeFunction(PackageName, ContractName, "testAddAccount",
                                  params, assets);

  params = std::map<std::string, std::string>();
  {
    params[tag::Uuid] = uuid;
    params[tag::AttachedAssetUuid] = "NEW ATTACHED UUID";
  }

  virtual_machine::invokeFunction(PackageName, ContractName, "testAttachAssetToAccount",
                                  params);

  const std::string received_serialized_acc =
      repository::world_state_repository::find(uuid);

  Api::Account account;
  account.ParseFromString(received_serialized_acc);

  ASSERT_STREQ("NEW ATTACHED UUID", params[tag::AttachedAssetUuid].c_str());

  // Remove cache again
  ASSERT_TRUE(repository::account::remove(uuid));
}

TEST(JavaQueryRepoAccount, invokeUpdateAccount) {

  /*****************************************************************
   * Remove cache & initialize
   *****************************************************************/
  const auto uuid =
      "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";

  if (repository::account::exists(uuid)) {
    repository::account::remove(uuid);
  }

  repository::account::add("MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=",
                           "MizukiSonoko", {"asset1", "asset2"});

  ASSERT_TRUE(repository::account::exists(uuid));

  /*****************************************************************
   * Invoke Java method
   *****************************************************************/
  const std::string FunctionName = "testUpdateAccount";

  std::map<std::string, std::string> params;
  {
    params[tag::Uuid] = uuid;
    params[tag::AccountName] = "Mi Nazuki Sonoko";
  }

  std::vector<std::string> assets;
  {
    assets.push_back("aaaaaa");
    assets.push_back("bbbbbb");
    assets.push_back("cccccc");
    assets.push_back("dddddd");
  }

  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName,
                                  params, assets);

  const std::string strAccount = repository::world_state_repository::find(uuid);

  Api::Account account;
  account.ParseFromString(strAccount);

  ASSERT_STREQ("MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=",
               account.publickey().c_str());
  ASSERT_STREQ(params[tag::AccountName].c_str(), account.name().c_str());
  for (std::size_t i = 0; i < assets.size(); i++) {
    ASSERT_STREQ(assets[i].c_str(), account.assets(i).c_str());
  }

  // Remove chache again
  repository::account::remove(uuid);
}

TEST(JavaQueryRepoAccount, invokeRemoveAccount) {

  /*****************************************************************
  * Remove cache & initialize
  *****************************************************************/
  const auto uuid =
      "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";

  std::map<std::string, std::string> params;
  {
    params[tag::Uuid] = uuid;
  }

  if (repository::account::exists(uuid)) {
    repository::account::remove(uuid);
  }

  repository::account::add("MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=",
                           "MizukiSonoko", {"asset1", "asset2"});

  ASSERT_TRUE(repository::account::exists(uuid));

  /*****************************************************************
   * Invoke Java method
   *****************************************************************/
  const std::string FunctionName = "testRemoveAccount";

  virtual_machine::invokeFunction(PackageName, ContractName, FunctionName,
                                  params);

  ASSERT_TRUE(!repository::account::exists(uuid));
}

TEST(JavaQueryRepoAccount, finishVM) {
  virtual_machine::finishVM(PackageName, ContractName);
}
