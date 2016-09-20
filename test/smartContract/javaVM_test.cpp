
#include "../../core/smartContract/javaVM.hpp"

#include <gtest/gtest.h>

TEST(SmartContract, createVM){
  std::unique_ptr<JavaContext> javaContext = initializeVM("SampleCurrency");
  if(javaContext != nullptr){ 
  }
}

TEST(SmartContract, CppToJava){
  std::unique_ptr<JavaContext> javaContext = initializeVM("SampleCurrency");
  if(javaContext != nullptr){ 
  }
}
