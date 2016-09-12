#ifndef CORE_CRYPTO_BASE64_HPP_
#define CORE_CRYPTO_BASE64_HPP_

#include <string>

namespace base64{
  std::string encode(const unsigned char*);
  const unsigned char* decode(std::string);
};

#endif  // CORE_CRYPTO_BASE64_HPP_
