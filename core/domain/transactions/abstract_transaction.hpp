#ifndef CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_

#include <stdio.h>

#include <string>
#include <vector>

namespace abstract_transaction {

enum TransactionType { 
    addPeer, modifyPeer, removePeer, transfer, signatory, signatoryAdd, signatoryDelete, domainDefinition,
    domainRenewal, aliasDefinition, aliasRenewal, assetDefinition, message, chaincodeInit, chaincodeInvoke,
    chaincodeUpdate, chaincodeDestroy, interchain
};

class AbstractTransaction {
public:
  virtual ~AbstractTransaction() = default; // make dtor virtual
    AbstractTransaction(AbstractTransaction&&) = default;  // support moving
    AbstractTransaction& operator = (AbstractTransaction&&) = default;
    AbstractTransaction(const AbstractTransaction&) = default; // support copying
    AbstractTransaction& operator = (const AbstractTransaction&) = default;

    virtual std::string getHash() = 0;
    virtual std::string getRawData() = 0;
    virtual std::string getAsText() = 0;
    virtual unsigned long long  getTimestamp() = 0;
    virtual TransactionType getType() = 0;
};
}  // namespace abstract_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_
