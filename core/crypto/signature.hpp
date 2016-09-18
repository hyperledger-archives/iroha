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


  std::string sign(
    std::string message,
    KeyPair  keyPair
  );

  std::string sign(
    std::string message,
    std::string publicKey,
    std::string privateKey
  );

  bool verify(
    const std::string signature,
    const std::string message,
    const std::string publicKey);

  KeyPair generateKeyPair();

};  // namespace signature

#endif  // CORE_CRYPTO_SIGNATURE_HPP_
