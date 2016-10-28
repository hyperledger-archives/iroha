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

    std::string lastInsertion = repository::world_state_repository::find("last_insertion");
    if (lastInsertion.empty()) {
        return newMerkleLeaf;
    }

    std::unique_ptr<MerkleNode> lastInsertionNode = convert(lastInsertion); //TODO: create convert function

    std::tuple<std::string, std::string> children = lastInsertionNode->parent->children;
    std::string right = std::get<1>(children);

    if (right.empty()) {
        // insert the event's transaction as the right child
        std::string left = std::get<0>(children);
        lastInsertionNode->parent->children = std::tuple<std::string, std::string>(left, event->tx->getAsText());

        // Propagate up the tree to the root
        lastInsertionNode->parent->hash = hash(left, event->tx->getHash());

        repository::world_state_repository::add(lastInsertionNode->parent->hash, lastInsertionNode->getAsText());// TODO: create getAsText
    } else {
        // create a new node and put it on the left
    }

    std::string currRoot = repository::world_state_repository::find("merkle_root");
    if (currRoot.empty()) {
        return newMerkleLeaf;
    }
    //TODO: convert currRoot string to MerkleNode
    std::unique_ptr<MerkleNode> currMerkleRoot = convert(currRoot);

    std::unique_ptr<MerkleNode> currNode;
    while (!currNode->isLeaf()) {
        // find insertion point for new node

    }

//            newMerkleLeaf->parent =

    return newMerkleRoot;
}
};  // namespace merkle_transaction_repository
