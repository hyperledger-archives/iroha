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

#ifndef CORE_DOMAIN_TRANSACTIONS_TRANSFERTRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_TRANSFERTRANSACTION_HPP_

#include "abstract_transaction.hpp"

namespace transaction {

class TransferTransaction : public abstract_transaction::AbstractTransaction {
    std::string prevTxHash;
    std::string hash;
    abstract_transaction::TransactionType type;
    std::string senderPublicKey;
    std::string receiverPublicKey;
    std::string domain;
    std::string asset;
    long long makotos;  // TODO: JS NUMBER range from -9007199254740992 to +9007199254740992 対応
    short int precision;
    unsigned long long timestamp;

public:
    TransferTransaction(abstract_transaction::AbstractTransaction &&, const std::string &prevTxHash);

    TransferTransaction(
        const std::string &senderPublicKey,
        const std::string &receiverPublicKey,
        const std::string &domain,
        const std::string &asset);

    virtual std::string getHash() const override;

    virtual std::string getRawData() const override;

    virtual std::string getAsText() const override;

    virtual unsigned long long int getTimestamp() const override;

    virtual abstract_transaction::TransactionType getType() const override;

    std::string getHash();
    std::string getRawData();
    std::string getAsText();
    unsigned long long  getTimestamp();
    abstract_transaction::TransactionType getType();
};

};  // namespace transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_TRANSFERTRANSACTION_HPP_

