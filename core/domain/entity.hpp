#ifndef CORE_DOMAIN_ENTITY_HPP_
#define CORE_DOMAIN_ENTITY_HPP_


#include <stdio.h>

#include <string>
#include <vector>

#include <msgpack.hpp>

class Entity {
 public:
    std::string uuid;
    std::vector<unsigned char> publicKeyVec;
    std::vector<unsigned char> privateKeyVec;

    unsigned char* publicKey;
    unsigned char* privateKey;

    MSGPACK_DEFINE(uuid, publicKeyVec, privateKeyVec);

    Entity():
      uuid(""),
      publicKey((unsigned char*)""),
      privateKey((unsigned char*)"")
    {}

    Entity(
      std::string aUuid,
      unsigned char* aPublicKey,
      unsigned char* aPrivateKey):
      uuid(aUuid),
      publicKey(aPublicKey),
      privateKey(aPrivateKey) {
      for (size_t i = 0; i < strlen(reinterpret_cast<char*>(publicKey) ); i++) {
        publicKeyVec.push_back(publicKey[i]);
      }
      for (
        size_t i = 0;
        i < strlen(reinterpret_cast<char*>(privateKey));
         i++) {
        privateKeyVec.push_back(privateKey[i]);
      }
    }
};

#endif  // CORE_DOMAIN_ENTITY_HPP_
