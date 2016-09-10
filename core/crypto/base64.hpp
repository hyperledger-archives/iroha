#ifndef CORE_CRYPTO_BASE64_HPP_
#define CORE_CRYPTO_BASE64_HPP_

#include <string>
#include <memory>

namespace base64{
  std::string encode(std::shared_ptr<unsigned char[]>);
  std::shared_ptr<unsigned char[]> decode(std::string);
};

#endif  // CORE_CRYPTO_BASE64_HPP_
