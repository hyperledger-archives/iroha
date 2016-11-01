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

#include <string>

namespace domain {

    Domain::Domain(
         const std::string &senderPublicKey, const std::string &receiverPublicKey,
         const std::string &domain, const std::string &asset
    ):
        senderPublicKey(senderPublicKey),
        receiverPublicKey(receiverPublicKey),
        domain(domain),
        asset(asset)
    {}

    std::string TransferTransaction::getHash() const {
        return hash::sha3_256_hex(senderPublicKey+receiverPublicKey+domain+asset);
    }

    std::string TransferTransaction::getRawData() const {
        return senderPublicKey+receiverPublicKey+domain+asset;
    }

    std::string TransferTransaction::getAsText() const {
        return senderPublicKey+receiverPublicKey+domain+asset;
    }

    unsigned long long int TransferTransaction::getTimestamp() const {
        return 0l;
    }

    transaction::TransactionType TransferTransaction::getType() const {
        return transaction::TransactionType::transfer;
    }

};  // namespace domain
