#ifndef CORE_CRYPTO_BASE64_HPP_
#define CORE_CRYPTO_BASE64_HPP_

#include <iostream>

#include <string>
#include <memory>
#include <vector>

#include <string.h>

namespace Base64{
  const std::string encode(const std::vector<unsigned char> message);
  std::vector<unsigned char> decode(const std::string enc);
};

#endif  // CORE_CRYPTO_BASE64_HPP_
