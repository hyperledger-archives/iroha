#ifndef CORE_CRYPTO_BASE64_HPP_
#define CORE_CRYPTO_BASE64_HPP_

#include <string>
#include <memory>

namespace base64{
  const std::string encode(const unsigned char*);
  std::unique_ptr<unsigned char> decode(std::string);
};

#endif  // CORE_CRYPTO_BASE64_HPP_
