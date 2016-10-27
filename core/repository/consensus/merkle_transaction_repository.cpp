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

#include "merkle_transaction_repository.hpp"
#include "../world_state_repository.hpp"

#include <memory>
#include <iostream>

#include "../../util/logger.hpp"
#include "../../crypto/merkle_node.hpp"
#include "../../crypto/merkle.hpp"

#include <string>

namespace merkle_transaction_repository {

using abs_tx = abstract_transaction::AbstractTransaction;

bool commit(const std::unique_ptr<consensus_event::ConsensusEvent> &event) {

    std::vector<std::tuple<std::string, std::string>> batchCommit
      = {
//            std::tuple<std::string, std::string>("lastOrder", tx->getAsText()),TODO: decide this
            std::tuple<std::string, std::string>(tx->getHash(), tx->getAsText())
    };

    return repository::world_state_repository::addBatch<
            std::string
        >(batchCommit);
}

bool leafExists(std::string const hash) {
    return !repository::world_state_repository::find(hash).empty();
}

std::string getLeaf(std::string const hash) {
    return repository::world_state_repository::find(hash);
}

unsigned long long getLastLeafOrder() {
    std::string lastAdded = repository::world_state_repository::lastAdded();
    //TODO: convert string->abstract transaction
    // return ->order; //TODO:
}

std::unique_ptr<MerkleNode> calculateNewRoot(const std::unique_ptr<consensus_event::ConsensusEvent> &event) {
    std::unique_ptr<MerkleNode> newMerkleLeaf = std::make_unique<MerkleNode>();
    std::unique_ptr<MerkleNode> newMerkleRoot = std::make_unique<MerkleNode>();

    newMerkleLeaf->hash = event->getHash();
    newMerkleLeaf->isLeaf()

    std::string currRoot = repository::world_state_repository::find("merkle_root");
    if (currRoot.empty()) {
        return newMerkleLeaf;
    }
    //TODO: convert currRoot to MerkleNode
    std::unique_ptr<MerkleNode> currMerkleRoot = convert(currRoot);

    while (c

    return newMerkleRoot;
}
};  // namespace merkle_transaction_repository
