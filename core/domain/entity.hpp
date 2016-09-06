#ifndef __ENTITY__
#define __ENTITY__

#include <msgpack.hpp>
#include <string>
#include <stdio.h>
#include <vector>

#include "../crypto/signature.hpp"

class Entity{
  public:
    std::string uuid;
    std::vector<unsigned char> publicKeyVec;
    std::vector<unsigned char> privateKeyVec;

    unsigned char* publicKey;
    unsigned char* privateKey;

    MSGPACK_DEFINE( uuid, publicKeyVec, privateKeyVec);

    Entity(std::string aUuid){
      uuid = aUuid;

    }
};

#endif
