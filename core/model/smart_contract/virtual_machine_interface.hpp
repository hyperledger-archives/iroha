#ifndef __CORE_SMART_CONTRACT_JAVA_VM_
#define __CORE_SMART_CONTRACT_JAVA_VM_

#include <unordered_map>
#include <string>

namespace smart_contract {

  template<typename T>
  class SmartContract {

    void initializeVM(std::string contractName);
    void finishVM();    
    void invokeFunction(
        std::string functionName,
          std::unordered_map<std::string, std::string> params);
    
  };
};

#endif  // __CORE_SMART_CONTRACT_JAVA_VM_