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

#include "../../core/model/smart_contract/virtual_machine_interface.hpp"
#include "../../core/repository/world_state_repository.hpp"
#include "../../core/crypto/hash.hpp"

#include <gtest/gtest.h>

using smart_contract::SmartContract;

const std::string PackageName       = "test";
const std::string ContractName      = "Test";
const std::string PublicKeyTag      = "publicKey";
const std::string DomainIdTag       = "domainId";
const std::string AccountNameTag    = "accountName";
const std::string AssetNameTag      = "assetName";
const std::string AssetValueTag     = "assetValue";

SmartContract smartContract = SmartContract();

TEST(SmartContract, InitializeVM) {
    smartContract.initializeVM(
        PackageName,
        ContractName
    );    
}

TEST(SmartContract, Invoke_JAVA_function) {

    const std::string FunctionName = "test1";

    smartContract.invokeFunction(
        PackageName,
        ContractName,
        FunctionName
    );
}

TEST(SmartContract, Invoke_JAVA_function_map_argv) {

    const std::string FunctionName = "test2";

    std::unordered_map<std::string, std::string> params;
    {
        params["key1"] = "Mizuki";
        params["key2"] = "Sonoko";
    }

    smartContract.invokeFunction(
        PackageName,
        ContractName,
        FunctionName,
        params
    );
}

TEST(SmartContract, Invoke_JAVA_function_map_utf_8) {

    const std::string FunctionName = "test3";

    std::unordered_map<std::string, std::string> params;
    {
        params["key1"] = "水樹";
        params["key2"] = "素子";
    }

    smartContract.invokeFunction(
        PackageName,
        ContractName,
        FunctionName,
        params
    );
}

TEST(SmartContract, Invoke_CPP_account_repo_function_FROM_JAVA_function) {

    const std::string FunctionName = "test_add_account";

    std::unordered_map<std::string, std::string> params;
    {
        params[PublicKeyTag]    = "MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=";
        params[AccountNameTag]  = "MizukiSonoko";
    }

    smartContract.invokeFunction(
        PackageName,
        ContractName,
        FunctionName,
        params
    );

    const std::string hashed_key =  // replace with UUID?
        "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";

    const std::string received_serialized_acc =
        repository::world_state_repository::find(hashed_key);

    ASSERT_STREQ(
        received_serialized_acc.c_str(),
        "\n,MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=\x12\fMizukiSonoko"//"MizukiSonoko"
    );
}

TEST(SmartContract, Invoke_CPP_asset_repo_function_FROM_JAVA_function) {

    const std::string FunctionName = "test_add_asset";

    std::unordered_map<std::string, std::string> params;
    {
        params[DomainIdTag]     = "A domain id";
        params[AssetNameTag]    = "Currency";
        params[AssetValueTag]   = "123456";
    }

    smartContract.invokeFunction(
        PackageName,
        ContractName,
        FunctionName,
        params
    );

    const std::string hashed_key =
        "3f8ba1e5df7f1587defc8fae4789207c8719c7b6d86ce299821b8a83fe08b5a9";

    // TODO: Getting uuid needs to be able to invoke non-void method.
    const std::string received_asset_value =
        repository::world_state_repository::find(hashed_key);

    ASSERT_STREQ(
        received_asset_value.c_str(),
        params[AssetValueTag].c_str()
    );

}

TEST(SmartContract, FinishVM) {
    smartContract.finishVM(
        PackageName,
        ContractName
    );    
}
