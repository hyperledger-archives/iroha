#ifndef CORE_DOMAIN_ABSTRACTTRANSACTION_HPP_
#define CORE_DOMAIN_ABSTRACTTRANSACTION_HPP_

#include <stdio.h>

#include <string>
#include <vector>

enum TransactionType { 
    addPeer, modifyPeer, removePeer, transfer, signatory, signatoryAdd, signatoryDelete, domainDefinition, 
    domainRenewal, aliasDefinition, aliasRenewal, assetDefinition, message, chaincodeInit, chaincodeInvoke, 
    chaincodeUpdate, chaincodeDestroy, interchain
};

class AbstractTransaction {
 public:
    std::string hash;
    std::string prevTxHash;  // Assume chains of transactions
    
    TransactionType type;
    
    std::vector<unsigned char> signature;

    std::string rawData;

    AbstractTransaction():
      hash(""),
      senderPublicKey((unsigned char*)""),
      privateKey((unsigned char*)"")
    {}

    AbstractTransaction(
      std::string hash,
      unsigned char* aPublicKey,
      unsigned char* aPrivateKey):
      publicKey(aPublicKey),
      privateKey(aPrivateKey) {

      for (size_t i = 0; i < strlen(reinterpret_cast<char*>(publicKey) ); ++i) {
        publicKeyVec.push_back(publicKey[i]);
      }
    }
};

#endif  // CORE_DOMAIN_ABSTRACTTRANSACTION_HPP_
