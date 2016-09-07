#ifndef __BASE64__
#define __BASE64__

#include <string>

namespace base64{
  std::string encode(const unsigned char*);
  unsigned char* decode(std::string);
};

#endif
