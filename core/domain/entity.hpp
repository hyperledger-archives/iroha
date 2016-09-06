#ifndef __ENTITY__
#define __ENTITY__

#include <msgpack.hpp>
#include <string>
#include <stdio.h>
#include <vector>

class Entity{
  public:
    std::string uuid;
    std::vector<unsigned char> publicKeyVec;
    std::vector<unsigned char> privateKeyVec;

    unsigned char* publicKey;
    unsigned char* privateKey;

    MSGPACK_DEFINE( uuid, publicKeyVec, privateKeyVec);

    Entity():
      uuid(""),
      publicKey((unsigned char*)""),
      privateKey((unsigned char*)"")
    {}

    Entity(
      std::string aUuid,
      unsigned char* aPublicKey,
      unsigned char* aPrivateKey
    ):
      uuid(aUuid),
      publicKey(aPublicKey),
      privateKey(aPrivateKey)
    {
      for(size_t i = 0;i < strlen((char*)publicKey);i++){
        publicKeyVec.push_back( publicKey[i] );
      }
      for(size_t i = 0;i < strlen((char*)privateKey);i++){
        privateKeyVec.push_back( privateKey[i] );
      }
    }
};

#endif
