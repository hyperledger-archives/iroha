#ifndef CORE_CRYPTO_HASH_HPP__
#define CORE_CRYPTO_HASH_HPP__

#include <string>

namespace Hash {
std::string sha3_256_hex(std::string message);
};

#endif  // CORE_CRYPTO_HASH_HPP_
