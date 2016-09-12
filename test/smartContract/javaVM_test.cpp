
#include "../../core/smartContract/javaVM.hpp"

#include <gtest/gtest.h>

TEST(SmartContract, createVM){
  std::unique_ptr<JavaContext> javaContext = createVM("test");
  execVM(javaContext);
}

