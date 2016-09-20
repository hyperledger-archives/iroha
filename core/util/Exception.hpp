#ifndef __EXCEPTIONS_
#define __EXCEPTIONS_

#include <stdexcept>
#include <string>

namespace exception {

  class FileOpenException : public std::invalid_argument {
    public: FileOpenException(const std::string&);
  };

  namespace crypto{
    class InvalidKeyException : public std::invalid_argument{
      public: InvalidKeyException(const std::string&);
    };
  };
};  // namespace exception

#endif
