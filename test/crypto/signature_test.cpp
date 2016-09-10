

#include "../../core/crypto/signature.hpp"
#include "../../core/crypto/base64.hpp"

#include <tuple>
#include <iostream>
#include <gtest/gtest.h>

#include <cstring>

TEST(Signature, E){
  std::tuple<
    unsigned char*,
    unsigned char*
  > keyPair = signature::generateKeyPair();
  
  std::cout << strlen((char*)std::get<0>(keyPair)) << std::endl;
  std::cout << strlen((char*)std::get<1>(keyPair)) << std::endl;
  std::cout << base64::encode(std::get<0>(keyPair)) << std::endl;
  std::cout << base64::encode(std::get<1>(keyPair)) << std::endl;
}


