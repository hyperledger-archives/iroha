#ifndef __BASE64_H_
#define __BASE64_H_

#include <string>

namespace base64{
  std::string encode(const unsigned char*);
  unsigned char* decode(std::string);
};
#endif
