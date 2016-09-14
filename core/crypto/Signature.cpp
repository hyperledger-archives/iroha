#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <algorithm>

#include "../domain/Entity.hpp"
#include "Signature.hpp"

#include "Base64.hpp"
#include "Hash.hpp"

namespace Signature {
  // === File Operation ===
  std::string loadKeyFile(std::string keyName) {
    std::ifstream ifs(keyName);
    if (ifs.fail()) {
      // ToDo Exception
    }
    return std::string(
      (std::istreambuf_iterator<char>(ifs) ),
      std::istreambuf_iterator<char>() );
  }

  std::shared_ptr<KeyPair> generateKeyPair(std::string filenamePrefix, std::string keyPath) {
    std::ofstream publicOfs(keyPath +"/"+ filenamePrefix + "_public.pem");
    std::ofstream privateOfs(keyPath +"/"+ filenamePrefix + "_private.pem");

    unsigned char publicKey[32], privateKey[64], seed[32];

    ed25519_create_seed(seed);
    ed25519_create_keypair(publicKey, privateKey, seed);

    publicOfs << Base64::encode(publicKey);
    privateOfs << Base64::encode(privateKey);
    std::cout << privateKey; //TODO:remove!

    std::shared_ptr<KeyPair> const keyPair = std::make_shared<KeyPair>(KeyPair(publicKey, privateKey));

    return keyPair;
  }

  std::string sign (
    std::string message,
    std::string privateKeyName,
    std::string publicKeyName) {
    auto privateKey = loadKeyFile(privateKeyName);
    auto publicKey  = loadKeyFile(publicKeyName);
    unsigned char signature[64];

    ed25519_sign (
      signature,
      reinterpret_cast<const unsigned char*>(message.c_str()),
      message.size(),
      reinterpret_cast<const unsigned char*>(Base64::decode(publicKey)),
      reinterpret_cast<const unsigned char*>(Base64::decode(privateKey)));
    return Base64::encode(signature);
  }

  bool verify (
    std::string signature,
    std::string message,
    std::string publicKeyName) {
      auto publicKey  = loadKeyFile(publicKeyName);
      return ed25519_verify(
        reinterpret_cast<const unsigned char*>(Base64::decode(signature)),
        reinterpret_cast<const unsigned char*>(message.c_str()),
        message.size(),
        reinterpret_cast<const unsigned char*>(Base64::decode(publicKey)));
  }
  // ===

};  // namespace signature
