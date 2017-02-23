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

#include <vm_interface/virtual_machine_interface.hpp>
#include <util/logger.hpp>
#include <map>
#include <string>

#include "jvm/java_virtual_machine.hpp"

namespace smart_contract {

    static std::map<std::string, std::unique_ptr<JavaContext>> vmSet;

    namespace detail {
        inline std::string pack(
            const std::string& packageName,
            const std::string& contractName
        ) {
            return packageName + "." + contractName;
        }
    }

    using detail::pack;

    void SmartContract::initializeVM(
        const std::string& packageName,
        const std::string& contractName
    ) {
        const auto NameId = pack(packageName, contractName);
        if (vmSet.find(NameId) != vmSet.end()) {
            // http://stackoverflow.com/questions/18486486/re-calling-jni-createjavavm-returns-1-after-calling-destroyjavavm
            logger::fatal("smart contract with java") << "not supported for initializing VM twice.";
            exit(EXIT_FAILURE);
//            vmSet.at(NameId)->jvm->DestroyJavaVM();
//            vmSet.erase(NameId);
        }
        vmSet.emplace(NameId, smart_contract::initializeVM(packageName, contractName));
    }

    void SmartContract::finishVM(
        const std::string& packageName,
        const std::string& contractName
    ) {
        const auto NameId = pack(packageName, contractName);
        if (vmSet.find(NameId) != vmSet.end()) {
            vmSet.at(NameId)->jvm->DestroyJavaVM();
//            vmSet.erase(NameId);
        }
    }

    void SmartContract::invokeFunction(
        const std::string& packageName,
        const std::string& contractName,
        const std::string& functionName,
        const std::unordered_map<std::string, std::string>& params
    ) {
        const auto NameId = pack(packageName, contractName);
        if (vmSet.find(NameId) != vmSet.end()){
            const auto& context = vmSet.at(NameId);
            smart_contract::execFunction(context, functionName, params);
        }
    }

    void SmartContract::invokeFunction(
        const std::string& packageName,
        const std::string& contractName,
        const std::string& functionName
    ) {
        const auto NameId = pack(packageName, contractName);
        if (vmSet.find(NameId) != vmSet.end()){
            const auto& context = vmSet.at(NameId);
            smart_contract::execFunction(context, functionName);
        }
    }
};
