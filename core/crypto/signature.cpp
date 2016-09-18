

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <algorithm>

#include "../domain/entity.hpp"
#include "signature.hpp"

#include "base64.hpp"
#include "hash.hpp"

namespace signature{

  bool verify(
    const std::string signature,
    const std::string message,
    const std::string publicKey){
    return ed25519_verify(
      base64::decode(signature).get(), 
      reinterpret_cast<const unsigned char*>(message.c_str()), 
      message.size(), 
      base64::decode(publicKey).get());
  }

  std::string sign(
    std::string message,
    KeyPair  keyPair
  ){
    unsigned char* signature = (unsigned char*)malloc(sizeof(unsigned char)*64);
    ed25519_sign(
      signature, 
      reinterpret_cast<const unsigned char*>(message.c_str()), 
      message.size(),
      keyPair.publicKey, 
      keyPair.privateKey);
    return base64::encode(signature);
  }

  std::string sign(
    std::string message,
    std::string publicKey,
    std::string privateKey
  ){
    unsigned char* signature = (unsigned char*)malloc(sizeof(unsigned char)*64);
    ed25519_sign(
      signature, 
      reinterpret_cast<const unsigned char*>(message.c_str()), 
      message.size(),
      base64::decode(publicKey).get(), 
      base64::decode(privateKey).get());
    return base64::encode(signature);
  }

  KeyPair generateKeyPair() {
    unsigned char* publicKey = (unsigned char*)malloc(sizeof(unsigned char)*32);
    unsigned char* privateKey= (unsigned char*)malloc(sizeof(unsigned char)*64);
    unsigned char* seed      = (unsigned char*)malloc(sizeof(unsigned char)*32);

    ed25519_create_seed(seed);
    ed25519_create_keypair(publicKey, privateKey, seed);

    return KeyPair( publicKey, privateKey);
  }
};  // namespace signature
