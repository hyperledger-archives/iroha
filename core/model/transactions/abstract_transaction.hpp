/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_

#include <stdio.h>

#include <string>
#include <vector>

namespace abstract_transaction {

enum class TransactionType { 
    addPeer, modifyPeer, removePeer, transfer, signatory, signatoryAdd, signatoryDelete, domainDefinition,
    domainRenewal, aliasDefinition, aliasRenewal, assetDefinition, message, chaincodeInit, chaincodeInvoke,
    chaincodeUpdate, chaincodeDestroy, interchain
};

class AbstractTransaction {
  public:
    std::string publicKey;
    std::string signature;

    AbstractTransaction(){};
    virtual ~AbstractTransaction() = default; // make dtor virtual

    AbstractTransaction(AbstractTransaction&&) = default;  // support moving
    AbstractTransaction& operator = (AbstractTransaction&&) = default;
    AbstractTransaction(const AbstractTransaction&) = default; // support copying
    AbstractTransaction& operator = (const AbstractTransaction&) = default;

    virtual std::string getHash() const = 0;
    virtual std::string getRawData() const = 0;
    virtual std::string getAsText() const = 0;
    virtual unsigned long long  getTimestamp() const = 0;
    virtual TransactionType getType() const  = 0;
};
}  // namespace abstract_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_
