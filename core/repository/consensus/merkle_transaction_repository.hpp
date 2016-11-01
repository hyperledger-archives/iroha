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

#ifndef CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_

#include <string>
#include <memory>
#include <unordered_map>
#include "../../consensus/consensus_event.hpp"

namespace merkle_transaction_repository {

struct MerkleNode {
    std::string hash;
    std::string parent;
    std::tuple<std::string, std::string> children;

    MerkleNode() {

    }

    MerkleNode(std::string jsonStr) {

    }

    bool isRoot() {
        return parent.empty();
    }

    bool isLeaf() {
        return std::get<0>(children).empty();
    }
};

void initLeaf();

bool commit(const std::unique_ptr<consensus_event::ConsensusEvent> &event);

bool leafExists(const std::string& hash);

std::string getLeaf(const std::string& hash);

std::unique_ptr<MerkleNode> calculateNewRoot(const std::unique_ptr<consensus_event::ConsensusEvent> &event);

};  // namespace merkle_transaction_repository

#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
