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
#include "../../crypto/hash.hpp"

#include <string>

namespace merkle_transaction_repository {

    bool commit(ConsensusEvent<T> &event) {
        std::vector<std::tuple<std::string, std::string>> batchCommit
          = {
                std::tuple<std::string, std::string>("last_insertion", event->tx->getHash()),
                std::tuple<std::string, std::string>(event->tx->getHash(), event->tx->getAsText())
        };

        //TODO: add in new leaf into the merkle tree

        return repository::world_state_repository::addBatch<std::string>(batchCommit);
    }

    bool leafExists(const std::string& hash) {
        return !repository::world_state_repository::find(hash).empty();
    }

    std::string getLeaf(const std::string& hash) {
        return repository::world_state_repository::find(hash);
    }

    std::string calculateNewRootHash(const std::unique_ptr<consensus_event::ConsensusEvent> &event) {
        std::unique_ptr<MerkleNode> newMerkleLeaf = std::make_unique<MerkleNode>();
        std::unique_ptr<MerkleNode> newMerkleRoot = std::make_unique<MerkleNode>();

        newMerkleLeaf->hash = event->getHash();

        std::string lastInsertionHash = repository::world_state_repository::find("last_insertion");
        if (lastInsertionHash.empty()) {
            return newMerkleLeaf;
        }

        std::string lastInsertionJSON = repository::world_state_repository::find(lastInsertionHash);
        MerkleNode lastInsertionNode = MerkleNode(lastInsertionJSON); //TODO: assume JSONParser wrapper

        std::tuple<std::string, std::string> children = lastInsertionNode.children;
        std::string right = std::get<1>(children);

        if (right.empty()) {
            // insert the event's transaction as the right child
            std::string left = std::get<0>(children);
            lastInsertionNode.children = std::tuple<std::string, std::string>(left, event->tx->getAsText());

//             lastInsertionNode.parent = hash::sha3_256_hex(left + event->tx->getHash());

        } else {
            // create a new node and put it on the left

        }

        // Propagate up the tree to the root
        std::unique_ptr<MerkleNode> currNode = lastInsertionNode.parent;
        std::unique_ptr<std::string> currHash;
        while (!currNode->isRoot()) {
            // find insertion point for new node
            currHash = currNode.hash;
            currNode = std::make_unique<MerkleNode>(currNode.parent);
            std::string currLeft = std::get<0>(currNode.children);
            currNode.hash = hash::sha3_256_hex(currLeft.hash + currHash);
        }

        return currNode.hash;
    }
};  // namespace merkle_transaction_repository
