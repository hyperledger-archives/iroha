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
#include "quque.hpp"

// ToDo This is MVP. so we should discuss how to implements this.
// In now, I use STL.
#include <queue>
#include <vector>
#include <functional>

namespace ordering{
    namespace queue{
        using Transaction = iroha::protocol::Transaction;
        using Block = iroha::protocol::Block;

        struct CompareTransaction : public std::binary_function<Transaction, Transaction, bool> {
            bool operator()(const Transaction& lhs, const Transaction& rhs) const
            {
                return lhs.header().created_time() < rhs.header().created_time();
            }
        };
        std::priority_queue<Transaction, std::vector<Transaction> , CompareTransaction> tx_queue;

        bool append(const Transaction& tx){
            tx_queue.push(tx);
            return true;
        }

        Block getBlock(){
            Block block;
            while(!tx_queue.empty()) {
                block.mutable_body()->add_txs()->CopyFrom(tx_queue.top());
                tx_queue.pop();
            }
            return block;
        }

        unsigned int getSize(){
            return tx_queue.size();
        }

    };
};
