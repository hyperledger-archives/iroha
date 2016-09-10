#ifndef CORE_DOMAIN_TRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTION_HPP_

#include <stdio.h>

#include <string>
#include <vector>

#include <msgpack.hpp>

enum TransactionType { 
    transfer, signatory, signatoryAdd, signatoryDelete, domainDefinition, assetDefinition, 
    message, chaincodeInit, chaincodeInvoke, chaincodeUpdate, chaincodeDestroy
};

class Transaction {
 public:
    std::string hash;
    
    TransactionType type;
    
    std::vector<unsigned char> senderPublicKey;
    std::vector<unsigned char> receiverPublicKey; //TODO(M->I): depending on the type, the applicable variables are different, so how should this be structured?

    MSGPACK_DEFINE(hash, type, senderPublicKey, receiverPublicKey); //TODO: http://stackoverflow.com/questions/39425975/defining-optional-parameters-using-msgpack-define-in-c14

    Transaction():
      hash(""),
      senderPublicKey((unsigned char*)""),
      privateKey((unsigned char*)"")
    {}

    Transaction(
      std::string hash,
      unsigned char* aPublicKey,
      unsigned char* aPrivateKey):
      uuid(aUuid),
      publicKey(aPublicKey),
      privateKey(aPrivateKey) {
          
      for (size_t i = 0; i < strlen(reinterpret_cast<char*>(publicKey) ); i++) {
        publicKeyVec.push_back(publicKey[i]);
      }
    }
};

#endif  // CORE_DOMAIN_TRANSACTION_HPP_
