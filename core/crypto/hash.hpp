#ifndef __HASH_H_
#define __HASH_H_

#include <string>

namespace hash{
  std::string sha3_256_hex(std::string message);
};
#endif
