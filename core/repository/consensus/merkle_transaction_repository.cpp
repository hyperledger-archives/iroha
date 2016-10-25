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

#include "merkle_transaction_repository.hpp"
#include "../world_state_repository.hpp"

#include <string>
#include <memory>
#include <iostream>

#include "../../util/logger.hpp"
#include "../../crypto/merkle_node.hpp"
#include "../../crypto/merkle.hpp"

namespace merkle_transaction_repository {

using abs_tx = abstract_transaction::AbstractTransaction;

std::vector<std::unique_ptr<abs_tx>> transactions;

// WIP
//std::unique_ptr<merkle::MerkleRoot> merkle_root;

bool commit(std::string hash, const std::unique_ptr<abs_tx> &tx) {
    return repository::world_state_repository::add(tx->getHash(), tx->getAsText());
}

bool leafExists(std::string const hash) {
    return !repository::world_state_repository::find(hash).empty();
}

};  // namespace merkle_transaction_repository
