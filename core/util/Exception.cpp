#include <stdexcept>
#include <string>

#include "Exception.hpp"

namespace Exception {

  FileOpenException::FileOpenException(const std::string& filename):
    std::invalid_argument("file " + filename +" is not found!")
  {}

  namespace crypto{
    InvalidKeyException::InvalidKeyException(const std::string& message):
      std::invalid_argument("keyfile is invalid cause:"+ message)
    {}
  };
};
