

#include "../../model/smart_contract/virtual_machine_interface.hpp"
#include "jvm/java_virtual_machine.hpp"
#include "../../util/logger.hpp"
#include <map>

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
