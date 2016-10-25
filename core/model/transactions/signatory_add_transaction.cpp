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

#include "signatory_add_transaction.hpp"

namespace signatory_add_transaction {
class SignatoryAddTransaction : public AbstractTransaction {
    std::string hash;
    AbstractTransaction::TransactionType type;

    std::string getHash() {
        return hash;
    }

    std::string getRawData() {
        //TODO
    }
    
    std::string getAsText() {
        //TODO
    }
    
    unsigned long long  getTimestamp() {
        return timestamp;
    }
    
    TransactionType getType() {
        return type;
    }
};

};  // namespace signatory_add_transaction