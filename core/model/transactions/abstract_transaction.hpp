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
    addPeer = 0,
    modifyPeer = 1,
    removePeer = 2,
    transfer = 3,
    signatory = 4,
    signatoryAdd = 5,
    signatoryDelete = 6,
    domainDefinition = 7,
    domainRenewal = 8,
    aliasDefinition = 9,
    aliasRenewal = 10,
    assetDefinition = 11,
    message = 12,
    chaincodeInit = 13,
    chaincodeInvoke = 14,
    chaincodeUpdate  = 15,
    chaincodeDestroy = 16,
    interchain = 17
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
    virtual std::string getAsJSON() const  = 0;
    virtual unsigned long long  getTimestamp() const = 0;
    virtual TransactionType getType() const  = 0;
};
}  // namespace abstract_transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_ABSTRACTTRANSACTION_HPP_
