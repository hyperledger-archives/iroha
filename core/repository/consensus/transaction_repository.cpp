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

#include "transaction_repository.hpp"

namespace repository{

    using Api::ConsensusEvent;
    using Api::Transaction;

    namespace transaction {

        void add(const std::string &key,const ConsensusEvent& tx){
            world_state_repository::add("transaction_" + key, tx.transaction().SerializeAsString());
        }

        std::vector<Transaction> findAll() {
            auto data = world_state_repository::findByPrefix("transaction_");
            std::vector<Transaction> res;
            for (auto &s: data) {
                Transaction tx;
                tx.ParseFromString(s);
                res.push_back(tx);
            }
            return res;
        }

        Transaction find(std::string key){
            std::string txKey = "transaction_" + key;
            Transaction tx;
            tx.ParseFromString(world_state_repository::find(txKey));
            return tx;
        }


        std::vector<Transaction> findByAssetName(std::string name){
            std::string txKey = "transaction_" + name + "_";
            auto data = world_state_repository::findByPrefix(txKey);
            std::vector<Transaction> res;
            for(auto& s: data){
                Transaction tx;
                tx.ParseFromString(s);
                res.push_back(tx);
            }
            return res;
        }

    };
};
