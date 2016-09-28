#include <stdexcept>
#include <string>

#include "exception.hpp"

namespace exception {

  FileOpenException::FileOpenException(const std::string& filename):
    std::invalid_argument("file " + filename +" is not found!")
  {}

  NotImplementedException::NotImplementedException(
    const std::string& functionName,
    const std::string& filename
  ):
    std::invalid_argument("ToDo. sorry [" + functionName +"] in "+filename+" is implemented, could you contribute to me?")
  {};

  namespace crypto {
    InvalidKeyException::InvalidKeyException(const std::string& message):
      std::invalid_argument("keyfile is invalid cause:"+ message)
    {}
  };  // namespace crypto

};  // namespace exception