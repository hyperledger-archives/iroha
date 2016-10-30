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

#include <json.hpp>

#include "../crypto/signature.hpp"

#include "../model/transactions/abstract_transaction.hpp"

namespace consensus_event {

using nlohmann::json;

struct ConsensusEvent {

    std::unique_ptr<abstract_transaction::AbstractTransaction> tx;
    std::unordered_map<std::string, std::string> txSignatures; // map of public keys→signatures

//    std::string merkleRootHash;
//    std::unordered_map<std::string, std::string> merkleRootSignatures;

    unsigned long long order = 0;

    ConsensusEvent(std::unique_ptr<abstract_transaction::AbstractTransaction> atx):
        tx(std::move(atx))
    {}

    explicit ConsensusEvent(std::string jsonStr) {
        json jsonObj = json::parse(jsonStr);

        tx = jsonObj.at(0);
        tsSignatures = jsonObj.at(1);

    }

    ConsensusEvent():
            tx(nullptr)
    {}

    void addSignature(const std::string& publicKey, const std::string& signature){
        txSignatures[publicKey] = signature;
    }

    std::string getHash() const {
        return tx->getHash();
    }

    int getNumValidSignatures() const {
        return std::count_if(
            txSignatures.begin(), txSignatures.end(),
            [hash = tx->getHash()](std::pair<const std::string, const std::string> record){
                return signature::verify(record.second, hash, record.first);
        });
    }

//    operator std::string() const{
//        return "WIP";
//    }

    std::string serializeToJSON() {
        json jsonObj;
        jsonObj.push_back(tx.serializeToJSON());
        jsonObj.push_back(txSignatures);

        return jsonObj.dump();
    }
};
};  // namespace ConsensusEvent

#endif  // CORE_CONSENSUS_CONSENSUSEVENT_HPP_