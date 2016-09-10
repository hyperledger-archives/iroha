#ifndef CORE_CRYPTO_SIGNATURE_HPP_
#define CORE_CRYPTO_SIGNATURE_HPP_

#include <string>
#include <memory>
#include <tuple>

#include <ed25519.h>

#include "../domain/entity.hpp"

namespace signature{
  
  class KeyPair{
   public:
    unsigned char* publicKey;
    unsigned char* privateKey;
    KeyPair(
      unsigned char* pub,
      unsigned char* pri
    ):
      publicKey(pub),
      privateKey(pri)
    {}
  };

  template<typename T>
  bool verify(
    std::string signature,
    std::string message,
    T dummy
  ) {
    // ToDo throw illegal type exception
  }
  template<typename T>
  std::string sign(std::string message, T dummy) {
    // ToDo throw illegal type exception
  }

  template<>
  bool verify<std::shared_ptr<Entity>>(
    const std::string signature,
    const std::string message,
    const std::shared_ptr<Entity> entity) {
    // ToDo
  }
  template<>
  std::string sign<std::shared_ptr<Entity>>(
    const std::string message,
    const std::shared_ptr<Entity> entity) {
    // ToDo
  }

  KeyPair generateKeyPair() {
    unsigned char publicKey[32], privateKey[64], seed[32];
    ed25519_create_seed(seed);
    ed25519_create_keypair(publicKey, privateKey, seed);
    return KeyPair( publicKey, privateKey);
  }
};  // namespace signature

#endif  // CORE_CRYPTO_SIGNATURE_HPP_
