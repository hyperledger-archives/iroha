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

#include "transfer_transaction.hpp"
#include <string>
#include "../../crypto/hash.hpp"
#include <json.hpp>

namespace message_transaction {

    using nlohmann::json;

    MessageTransaction::MessageTransaction(
         const std::string &senderPublicKey, const std::string &receiverPublicKey,
         const std::string &data
    ):
        AbstractTransaction(),
        senderPublicKey(senderPublicKey),
        receiverPublicKey(receiverPublicKey),
        data(data)
    {}

    std::string MessageTransaction::getHash() const {
        return hash::sha3_256_hex(senderPublicKey + receiverPublicKey + domain + asset);
    }

    std::string MessageTransaction::getRawData() const {
        return senderPublicKey + receiverPublicKey + data;
    }

    std::string MessageTransaction::getAsText() const {
        return senderPublicKey + receiverPublicKey + data;
    }

    std::string MessageTransaction::getAsJSON() const {
        json jsonObj;
        //TODO: only serialize the type and data for now
        jsonObj.push_back(getType());
        jsonObj.push_back(data);

        return jsonObj.dump();
    }

    unsigned long long int MessageTransaction::getTimestamp() const {
        return 0l;
    }

    abstract_transaction::TransactionType MessageTransaction::getType() const {
        return abstract_transaction::TransactionType::message;
    }

};  // namespace transaction
