#ifndef CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_

#include <stdio.h>

#include <string>
#include <vector>

namespace AbstractTransaction {
  std::string getRawData();

  enum TransactionType { 
      addPeer, modifyPeer, removePeer, transfer, signatory, signatoryAdd, signatoryDelete, domainDefinition,
      domainRenewal, aliasDefinition, aliasRenewal, assetDefinition, message, chaincodeInit, chaincodeInvoke,
      chaincodeUpdate, chaincodeDestroy, interchain
  };

  class AbstractTransaction {
  public:
      // unsigned long long id;
      unsigned long long timestamp;
      std::string hash;// TODO: add merkle node here?
      std::string prevTxHash;  // Assume chains of transactions
      TransactionType type;
      std::vector<unsigned char> signature;
      // Base64 signature;

      std::string getRawData();

      AbstractTransaction(
        std::string hash,
        std::vector<unsigned char> signature,
        privateKey(aPrivateKey) {

      }
  };
}  // namespace AbstractTransaction

#endif  // CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_
