

#include "../../core/crypto/signature.hpp"
#include "../../core/crypto/base64.hpp"

#include <tuple>
#include <iostream>
#include <gtest/gtest.h>

#include <cstring>

TEST(Signature, E){
  signature::KeyPair keyPair = signature::generateKeyPair();
  
  std::cout << strlen((char*)keyPair.publicKey) << std::endl;
  std::cout << strlen((char*)keyPair.privateKey) << std::endl;
  std::cout << base64::encode(keyPair.publicKey) << std::endl;
  std::cout << base64::encode(keyPair.privateKey) << std::endl;
}


