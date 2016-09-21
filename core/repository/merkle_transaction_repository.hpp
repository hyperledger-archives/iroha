#ifndef CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_

#include <string>
#include <msgpack.hpp>
#include "../domain/transactions/abstract_transaction.hpp"
#include "../domain/consensus/consensus_event.hpp"

namespace merkle_transaction_repository {
bool commit(std::string const hash, ConsensusEvent const tx);
MerkleNode find(std::string const hash);
};  // namespace merkle_transaction_repository

#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
