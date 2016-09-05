#ifndef __ENTITY__
#define __ENTITY__

#include <string>

class Entity{
  public:
    std::string uuid;
    unsigned char* publicKey;
    unsigned char* privateKey;
};

#endif
