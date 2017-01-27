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

#include <gtest/gtest.h>

using smart_contract::SmartContract;

const std::string contractName = "Test";

TEST(SmartContract, InitializeVM){
    SmartContract smartContract = SmartContract();
    smartContract.initializeVM(contractName);
}

TEST(SmartContract, Invoke_JAVA_function){
    const std::string functionName = "test1";
    SmartContract smartContract = SmartContract();
    smartContract.invokeFunction(
        contractName,
        functionName
    );
}

TEST(SmartContract, Invoke_JAVA_function_map_argv){
    const std::string functionName = "test2";
    std::unordered_map<std::string, std::string> params;
    params["key1"] = "Mizuki";
    params["key2"] = "Sonoko";
    SmartContract smartContract = SmartContract();
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
}

TEST(SmartContract, Invoke_JAVA_function_map_utf_8){
    const std::string functionName = "test3";
    std::unordered_map<std::string, std::string> params;
    params["key1"] = "水樹";
    params["key2"] = "素子";
    SmartContract smartContract = SmartContract();
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
}

TEST(SmartContract, Invoke_CPP_account_repo_function_FROM_JAVA_function) {
    const std::string functionName = "test_add_account";
    std::unordered_map<std::string, std::string> params;
    params["publicKey"]     = "MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=";
    params["accountName"]   = "MizukiSonoko";
    std::string hashed_key = "eeeada754cb39bff9f229bca75c4eb8e743f0a77649bfedcc47513452c9324f5";
    std::string serialized_string = "\n,MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=\x12\fMizukiSonoko";
//    params["publicKey"] = "PUBLICKEY";
//    std::string hashed_key = "3f31d574eb12fd73b1a0b6c9614ef3e22649c5ad80e6e736a5aa82e8606b8971";

    SmartContract smartContract = SmartContract();
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );

    std::string value = repository::world_state_repository::find(hashed_key);
    ASSERT_STREQ(
      value.c_str(),
      serialized_string.c_str()//"MizukiSonoko"
    );
}

TEST(SmartContract, Invoke_CPP_asset_repo_function_FROM_JAVA_function){
    const std::string functionName = "test_add_asset";
    std::unordered_map<std::string, std::string> params;
    params["publicKey"]     = "public key asset";
    params["assetName"]     = "asset name";
    params["assetValue"]    = "asset value";
    std::string hashed_key = "3f31d574eb12fd73b1a0b6c9614ef3e22649c5ad80e6e736a5aa82e8606b8971";

    SmartContract smartContract = SmartContract();
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );

    std::string value = repository::world_state_repository::find(params["assetName"]+"@"+params["publicKey"]);
    ASSERT_STREQ(
      value.c_str(),
      params["assetValue"].c_str()
    );
}

TEST(SmartContract, FinishVM){
    SmartContract smartContract = SmartContract();
    smartContract.finishVM(contractName);
}
