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

#ifndef CORE_DOMAIN_TRANSACTIONS_MESSAGETRANSACTION_HPP_
#define CORE_DOMAIN_TRANSACTIONS_MESSAGETRANSACTION_HPP_

#include "abstract_transaction.hpp"

namespace message_transaction {

class MessageTransaction : public abstract_transaction::AbstractTransaction {
    std::string hash;
    abstract_transaction::TransactionType type;
    std::string senderPublicKey;
    std::string receiverPublicKey;
    std::string data;
    unsigned long long timestamp;

public:
//    MessageTransaction(abstract_transaction::AbstractTransaction &&, const std::string &prevTxHash);

    MessageTransaction(const std::string &data);

    virtual std::string getHash() const override;

    virtual std::string getRawData() const override;

    virtual std::string getAsText() const override;

    virtual unsigned long long int getTimestamp() const override;

    virtual abstract_transaction::TransactionType getType() const override;

    std::string getHash();
    std::string getRawData();
    std::string getAsText();
    std::string getAsJSON();
    unsigned long long  getTimestamp();
    abstract_transaction::TransactionType getType();
};

};  // namespace transaction

#endif  // CORE_DOMAIN_TRANSACTIONS_MESSAGETRANSACTION_HPP_

