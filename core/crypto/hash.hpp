#ifndef CORE_CRYPTO_HASH_HPP__
#define CORE_CRYPTO_HASH_HPP__

#include <string>

namespace base64{
  std::string encode(const unsigned char*);
  unsigned char* decode(std::string);
};

#endif  // CORE_CRYPTO_HASH_HPP_
