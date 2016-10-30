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
#include "../../util/random.hpp"

#include <memory>
#include <iostream>

#include "../../util/logger.hpp"
//#include "../../crypto/merkle_node.hpp"
//#include "../../crypto/merkle.hpp"
#include "../../crypto/hash.hpp"

#include <string>

namespace merkle_transaction_repository {

    using abs_tx = abstract_transaction::AbstractTransaction;

    bool commit(const std::unique_ptr<consensus_event::ConsensusEvent> &event) {

        std::vector<std::tuple<std::string, std::string>> batchCommit
          = {
                std::tuple<std::string, std::string>("last_insertion", event->tx->getHash()),
                std::tuple<std::string, std::string>(event->tx->getHash(), event->tx->getAsText())
        };

        return repository::world_state_repository::addBatch<
                std::string
        >(batchCommit);
    }

    bool leafExists(const std::string& hash) {
        return !repository::world_state_repository::find(hash).empty();
    }

    std::string getLeaf(const std::string& hash) {
        return repository::world_state_repository::find(hash);
    }

    unsigned long long getLastLeafOrder() {
        std::string lastAddedHash = repository::world_state_repository::find("last_insertion");
        if (lastAddedHash.empty()) {
            return 0l;
        }

        std::string lastAddedJSON = repository::world_state_repository::find(lastAddedHash);
        if (lastAddedJSON.empty()) {
            return 0l;
        }

        auto lastAddedLeaf = json::parse(lastAddedJSON);

        return lastAddedLeaf['order'];
    }

    void initLeaf(){
        repository::world_state_repository::add("last_insertion", random_service::makeRandomHash());
    }

    std::unique_ptr<MerkleNode> calculateNewRoot(const std::unique_ptr<consensus_event::ConsensusEvent> &event) {
        std::unique_ptr<MerkleNode> newMerkleLeaf = std::make_unique<MerkleNode>();
        std::unique_ptr<MerkleNode> newMerkleRoot = std::make_unique<MerkleNode>();

        newMerkleLeaf->hash = event->getHash();

        std::string lastInsertionHash = repository::world_state_repository::find("last_insertion");
        if (lastInsertionHash.empty()) {
            return newMerkleLeaf;
        }

        std::string lastInsertionJSON = repository::world_state_repository::find(lastInsertionHash);
        MerkleNode lastInsertionNode = MerkleNode(lastInsertionJSON); //TODO: create convert function

        std::tuple<std::string, std::string> children = lastInsertionNode.children;
        std::string right = std::get<1>(children);

        if (right.empty()) {
            // insert the event's transaction as the right child
            std::string left = std::get<0>(children);
            lastInsertionNode.children = std::tuple<std::string, std::string>(left, event->tx->getAsText());

            // Propagate up the tree to the root
            // std::unique_ptr<MerkleNode> currNode = lastInsertionNode.parent;
            // while (!currNode->isRoot()) {
                // find insertion point for new node
            // }
            // lastInsertionNode.parent = hash::sha3_256_hex(left + event->tx->getHash());

        } else {
            // create a new node and put it on the left

        }

        const std::string currRootJSON = repository::world_state_repository::find("merkle_root");
        if (currRootJSON.empty()) {
            return newMerkleLeaf;
        }

        MerkleNode currMerkleRoot = MerkleNode(currRootJSON);

        return newMerkleRoot;
    }
};  // namespace merkle_transaction_repository
