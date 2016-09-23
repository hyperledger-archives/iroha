#ifndef CORE_CRYPTO_SIGNATURE_HPP_
#define CORE_CRYPTO_SIGNATURE_HPP_

#include <string>
#include <memory>
#include <vector>

namespace signature {

  class KeyPair {
   public:
    std::vector<unsigned char> publicKey;
    std::vector<unsigned char> privateKey;
    KeyPair(
      std::vector<unsigned char>&& pub,
      std::vector<unsigned char>&& pri
    ):
      publicKey(std::move(pub)),
      privateKey(std::move(pri))
    {}
  };

  std::string sign(
    std::string message,
    KeyPair  keyPair
  );

  std::string sign(
    std::string message,
    std::string publicKey_b64,
    std::string privateKey_b64
  );

  bool verify(
    const std::string signature_b64,
    const std::string message,
    const std::string publicKey_b64);

  KeyPair generateKeyPair();

};  // namespace signature

#endif  // CORE_CRYPTO_SIGNATURE_HPP_
