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
#include <repository/transaction_repository.hpp>

namespace repository{

    using Api::Transaction;

    namespace transaction {

        bool add(const std::string &key,const Transaction& tx){
            return world_state_repository::add("transaction_" + key, tx.SerializeAsString());
        }

        std::vector<Transaction> findAll(){
            std::vector<Transaction> res;
            auto stx = world_state_repository::findByPrefix("transaction_");
            for(auto&& st: stx){
                Transaction tx;
                tx.ParseFromString(st);
                res.emplace_back(tx);
            }
            return res;
        }

        Transaction find(const std::string& key){
            Transaction tx;
            auto st = world_state_repository::find("transaction_"+key);
            if(!st.empty()){
                tx.ParseFromString(st);
            }
            return tx;
        }

    }
}
