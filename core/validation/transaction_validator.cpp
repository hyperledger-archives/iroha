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

#include <algorithm>
#include <infra/protobuf/api.pb.h>
#include <crypto/signature.hpp>
#include "transaction_validator.hpp"

namespace transaction_validator {

    using Api::ConsensusEvent;
    using Api::Transaction;

    template<>
    bool signaturesAreValid<ConsensusEvent>(const std::unique_ptr<ConsensusEvent>& tx){
        return tx->eventsignatures().end() != std::find_if(tx->eventsignatures().begin(), tx->eventsignatures().end(), 
            [&tx](const auto &sig) {
                return !signature::verify(sig.signature(), tx->transaction().hash(), sig.publickey());
            });
    }

    template<>
    bool signaturesAreValid<Transaction>(const std::unique_ptr<Transaction>& tx){
        return tx->txsignatures().end() != std::find_if(tx->txsignatures().begin(), tx->txsignatures().end(), 
            [&tx](const auto &sig) {
                return !signature::verify(sig.signature(), tx->hash(), sig.publickey());
            });
    }

};
