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

#include <gtest/gtest.h>

using smart_contract::SmartContract;

/*
TEST(SmartContractFromJava, test1){
    std::string contractName = "JavaExecuteTest";
    std::string functionName = "test1";
    SmartContract smartContract = SmartContract();
    smartContract.initializeVM(contractName);
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
    smartContract.finishVM(contractName);
}
*/
/*
TEST(SmartContractFromJava, test2){
    std::string contractName = "JavaExecuteTest";
    std::string functionName = "test2";
    std::unordered_map<std::string, std::string> params;
    params["key1"] = "Mizuki";
    params["key2"] = "Sonoko";
    SmartContract smartContract = SmartContract();
    smartContract.initializeVM(contractName);
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
    smartContract.finishVM(contractName);
}

TEST(SmartContractFromJava, test3){
    std::string contractName = "JavaExecuteTest";
    std::string functionName = "test3";
    std::unordered_map<std::string, std::string> params;
    params["key1"] = "水樹";
    params["key2"] = "素子";
    SmartContract smartContract = SmartContract();
    smartContract.initializeVM(contractName);
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
    smartContract.finishVM(contractName);
}
*/
TEST(SmartContractFromJava, test4){
    std::string contractName = "JavaExecuteTest";
    std::string functionName = "test4";
    std::unordered_map<std::string, std::string> params;
    params["key"] = "MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=";
    SmartContract smartContract = SmartContract();
    smartContract.initializeVM(contractName);
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
    smartContract.finishVM(contractName);
}

TEST(SmartContractFromJava, test5){
    std::string contractName = "JavaExecuteTest";
    std::string functionName = "test5";
    std::unordered_map<std::string, std::string> params;
    params["key"] = "MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=";
    SmartContract smartContract = SmartContract();
    smartContract.initializeVM(contractName);
    smartContract.invokeFunction(
        contractName,
        functionName,
        params
    );
    smartContract.finishVM(contractName);
}
