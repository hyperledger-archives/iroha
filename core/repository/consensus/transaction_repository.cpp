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

#include "../world_state_repository.hpp"
#include "transaction_repository.hpp"
#include "../../crypto/base64.hpp"
#include <string>
#include <vector>

namespace repository{

    namespace transaction {

        void add(std::string &key, std::string strTx){
            std::unique_ptr<unsigned char[]> arrayTx(new unsigned char[sizeof(char)*strTx.size()]);
            for(unsigned int i = 0;i < strTx.size();i++){
                arrayTx[i] = strTx[i];
            }
            std::vector<unsigned char> vecTx(strTx.size());
            vecTx.assign(arrayTx.get(),arrayTx.get()+strTx.size());
            logger::debug("repository/transaction/add", "base64:[[[[[["+base64::encode(vecTx)+"]]]]]]");
            world_state_repository::add("transaction_" + key, base64::encode(vecTx));
        }

        std::vector<std::string> findAll(){
            auto data = world_state_repository::findByPrefix("transaction_");
            std::vector<std::string> res;
            for(auto& s: data){
                logger::debug("repository/transaction/findAll", "base64:"+s);
                auto vecTx = base64::decode(s);
                std::unique_ptr<unsigned char[]> arrayTx(new unsigned char[sizeof(unsigned char)*vecTx.size()+1]);
                size_t pos = 0;
                for(auto c : vecTx){
                    arrayTx.get()[pos] = c;pos++;
                }
                arrayTx.get()[pos] = '\0';
                res.push_back(std::string(reinterpret_cast<char*>(arrayTx.get())));
            }
            return res;
        }

        std::vector<std::string> findByAssetName(std::string name){
            auto data = world_state_repository::findByPrefix("transaction_"+name+"_");
            std::vector<std::string> res;/*
            for(auto& s: data){
                res.push_back(base64::decode(s));
            }
            */
            return res;
        }

    };
};