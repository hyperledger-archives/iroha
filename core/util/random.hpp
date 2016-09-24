#ifndef __RANDOM_HPP_
#define __RANDOM_HPP_

namespace random{

  #include <random>
  #include "../crypto/hash.hpp"
  std::string makeRandomHash(){
    std::random_device rd;
    return hash::sha3_256_hex(std::to_string(rd()));  
  }

};  // namespace random

#endif
