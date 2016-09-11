#ifndef CORE_DOMAIN_TRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTION_HPP_

#include <stdio.h>

#include <string>
#include <vector>

#include <msgpack.hpp>

enum TransactionType { 
    transfer, signatory, signatoryAdd, signatoryDelete, domainDefinition, domainRenewal, aliasDefinition, aliasRenewal,
    assetDefinition, message, chaincodeInit, chaincodeInvoke, chaincodeUpdate, chaincodeDestroy
};

class AbstractTransaction {
 public:
    std::string hash;
    std::string prevTxHash;  // Assume chains of transactions
    
    TransactionType type;
    
    std::vector<unsigned char> senderPublicKey;
    std::vector<unsigned char> receiverPublicKey; //TODO(M->I): depending on the type, the applicable variables are different, so how should this be structured?

    AbstractTransaction():
      hash(""),
      senderPublicKey((unsigned char*)""),
      privateKey((unsigned char*)"")
    {}

    AbstractTransaction(
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
