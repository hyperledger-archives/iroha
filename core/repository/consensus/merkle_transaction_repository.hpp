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
#include "../../model/transactions/abstract_transaction.hpp"
#include "../../consensus/consensus_event.hpp"
#include <json.hpp>

namespace merkle_transaction_repository {

using nlohmann::json;

struct MerkleNode {
    std::string hash;
    std::string parent;
    std::tuple<std::string, std::string> children;

    explicit MerkleNode(std::string jsonStr) {
        json jsonObj = json::parse(jsonStr);
        hash = jsonObj.at(0);
        parent = jsonObj.at(1);
        std::string leftChild = jsonObj.at(2);
        children = std::tuple<std::string, std::string>(std::string(jsonObj.at(2)), std::string(jsonObj.at(3)));
    }

    MerkleNode() {

    }

    bool isRoot() {
        return parent.empty();
    }

    bool isLeaf() {
        return std::get<0>(children).empty();
    }

    std::string serializeToJSON() {
        json jsonObj;
        jsonObj.push_back(hash);
        jsonObj.push_back(parent);
        jsonObj.push_back(std::get<0>(children));
        jsonObj.push_back(std::get<1>(children));

        return jsonObj.dump();
    }
};

void initLeaf();

bool commit(const std::unique_ptr<consensus_event::ConsensusEvent> &event);

bool leafExists(const std::string& hash);

std::string getLeaf(const std::string& hash);

unsigned long long getLastLeafOrder();

std::unique_ptr<MerkleNode> calculateNewRoot(const std::unique_ptr<consensus_event::ConsensusEvent> &event);

};  // namespace merkle_transaction_repository

#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
