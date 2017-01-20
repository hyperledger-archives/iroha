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

TEST(SmartContract, Invoke_JAVA_function){
    std::string contractName = "Test";
    std::string functionName = "test1";
    SmartContract smartContract = SmartContract();
    smartContract.initializeVM(contractName);
    smartContract.invokeFunction(
        contractName,
        functionName
    );
}

TEST(SmartContract, Invoke_JAVA_function_map_argv){
    std::string contractName = "Test";
    std::string functionName = "test2";
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
    std::string contractName = "Test";
    std::string functionName = "test3";
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

TEST(SmartContract, Invoke_CPP_function_FROM_JAVA_function){
    std::string contractName = "Test";
    std::string functionName = "test4";
    std::unordered_map<std::string, std::string> params;
    params["key"] = "PUBLICKEY";
    std::string hashed_key = "3f31d574eb12fd73b1a0b6c9614ef3e22649c5ad80e6e736a5aa82e8606b8971";

    SmartContract smartContract = SmartContract();
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
    smartContract.finishVM(contractName);

    std::string value = repository::world_state_repository::find(hashed_key);
    ASSERT_STREQ(
      value.c_str(),
      "MizukiSonoko"
    );
}
