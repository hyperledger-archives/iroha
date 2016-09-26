#include "../../../repository/consensus/merkle_transaction_repository.hpp"

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <string>
#include <memory>
#include <iostream>

#include "../../../util/logger.hpp"
#include "../../../crypto/merkle_node.hpp"
#include "../../../crypto/merkle.hpp"

namespace merkle_transaction_repository {

using abs_tx = abstract_transaction::AbstractTransaction;

std::shared_ptr<leveldb::DB> db;
// WIP
//std::unique_ptr<merkle::MerkleRoot> merkle_root;

bool printStatus(leveldb::Status const status) {
}

std::unique_ptr<abs_tx> convertTransaction(std::string const buffer) {
}
                                                       
std::string convertBuffer(std::unique_ptr<abs_tx> tx) {
}

void loadDb() {
}

bool commit(std::shared_ptr<consensus_event::ConsensusEvent> event) {
}

bool commit(std::string, std::shared_ptr<consensus_event::ConsensusEvent> event) {
}

std::unique_ptr<abs_tx> findLeaf(std::string const hash) {
}
};  // namespace merkle_transaction_repository
