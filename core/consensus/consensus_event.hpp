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

#ifndef CORE_CONSENSUS_CONSENSUSEVENT_HPP_
#define CORE_CONSENSUS_CONSENSUSEVENT_HPP_

#include <string>
#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <algorithm>

#include "../crypto/signature.hpp"
#include "../util/logger.hpp"
#include "../model/transaction.hpp"

namespace consensus_event {

template<
        typename T,
        std::enable_if_t<
                std::is_base_of<transaction::Transaction, T>::value,std::nullptr_t
        > = nullptr
>
class ConsensusEvent {


    struct eventSignature{
        std::string publicKey;
        std::string signature;

        eventSignature(pubKey,sig):
                publicKey(pubKey),
                signature(sig)
        {}
    };
    T tx;
    std::vector<eventSignature> eventSignatures;

private:
    unsigned long long order = 0;

    ConsensusEvent(T atx):
        tx(atx)
    {}

    void addSignature(const std::string& publicKey, const std::string& signature){
        eventSignatures.push_back(eventSignature(publicKey, signature));
    }

    std::string getHash() const {
        return tx->getHash();
    }

    int getNumValidSignatures() const {
        logger::debug("getNumValidSignatures", "eventSignatures:"+ std::to_string(eventSignatures.size()));
        return std::count_if(
            txSignatures.begin(), txSignatures.end(),
            [hash = tx->getHash()](eventSignature sig){
                return signature::verify(sig.signature, hash, sig.publicKey);
        });
    }

    std::string serializeToJSON() {

    }
};
};  // namespace ConsensusEvent

#endif  // CORE_CONSENSUS_CONSENSUSEVENT_HPP_