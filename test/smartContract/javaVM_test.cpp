
#include "../../core/smartContract/javaVM.hpp"

#include <gtest/gtest.h>

TEST(SmartContract, createVM){
  std::unique_ptr<JavaContext> javaContext = createVM("SampleCurrency");
  if(javaContext != nullptr){ 
    execVM(javaContext);
  }
}

TEST(SmartContract, CppToJava){
  std::unique_ptr<JavaContext> javaContext = createVM("SampleCurrency");
  if(javaContext != nullptr){ 
    execVM(javaContext);
  }
}
