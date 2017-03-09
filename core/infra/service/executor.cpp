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

#include <infra/protobuf/api.pb.h>
#include <iostream>

namespace executor{

    using Api::Transaction;

    void add(const Transaction& tx){
        if(tx.has_asset()){
            // Add<Asset>
        }else if(tx.has_domain()){
            // Add<Domain>
        }else if(tx.has_account()){
            // Add<Account>
        }else if(tx.has_peer()){
            // Add<Peer>
        }
    }

    void transfer(const Transaction& tx){
        if(tx.has_asset()){
            // Transfer<Asset>
        }else if(tx.has_domain()){
            // Transfer<Domain>
        }else if(tx.has_account()){
            // Transfer<Account>
        }else if(tx.has_peer()){
            // Transfer<Peer>
        }
    }

    void update(const Transaction& tx){
        if(tx.has_asset()){
            // Update<Asset>
        }else if(tx.has_domain()){
            // Update<Domain>
        }else if(tx.has_account()){
            // Update<Account>
        }else if(tx.has_peer()){
            // Update<Peer>
        }
    }

    void remove(const Transaction& tx){
        if(tx.has_asset()){
            // Remove<Asset>
        }else if(tx.has_domain()){
            // Remove<Domain>
        }else if(tx.has_account()){
            // Remove<Account>
        }else if(tx.has_peer()){
            // Remove<Peer>
        }
    }

    void contract(const Transaction& tx){
        if(tx.has_asset()){
            // Contract<Asset>
        }else if(tx.has_domain()){
            // Contract<Domain>
        }else if(tx.has_account()){
            // Contract<Account>
        }else if(tx.has_peer()){
            // Contract<Peer>
        }
    }

    void execute(const Transaction& tx){
        std::cout << "Executor\n";
        std::cout << "DebugString:"<< tx.DebugString() <<"\n";
        if(tx.type() == "add"){
            add(tx);
        }else if(tx.type() == "transfer"){
            transfer(tx);
        }else if(tx.type() == "update"){
            update(tx);
        }else if(tx.type() == "remove"){
            remove(tx);
        }else if(tx.type() == "contract"){
            contract(tx);
        }
    }

};
