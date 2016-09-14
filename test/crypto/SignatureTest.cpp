#include "../../core/crypto/Signature.hpp"
#include "../../core/crypto/Base64.hpp"

#include <tuple>
#include <iostream>
#include <gtest/gtest.h>

#include <cstring>

TEST(Signature, E){
  std::shared_ptr<Signature::KeyPair> keyPair = Signature::generateKeyPair();
  
  std::cout << strlen((char*)keyPair->publicKey) << std::endl;
  std::cout << strlen((char*)keyPair->privateKey) << std::endl;
  std::cout << Base64::encode(keyPair->publicKey) << std::endl;
  std::cout << Base64::encode(keyPair->privateKey) << std::endl;
}
