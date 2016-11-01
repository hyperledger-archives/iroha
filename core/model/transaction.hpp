/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp
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
#ifndef CORE_DOMAIN_TRANSACTIONS_TRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_TRANSACTION_HPP_

namespace transaction {

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

template <typename T>
class Transaction {
    T command;
private:
    std::hash;

public:
    virtual ~Transaction() = default;

    // Support move and copy.
    Transaction(Transaction const&) = default;
    Transaction(Transaction&&) = default;
    Transaction& operator =(Transaction const&) = default;
    Transaction& operator =(Transaction&&) = default;

    std::string getHash() {
        return object.getHash();
    }

    std::string getAsJSON() {
        return T.getAsJSON();
    }
};

}  // namespace transaction

//#endif  // CORE_DOMAIN_TRANSACTIONS_TRANSACTION_HPP_
