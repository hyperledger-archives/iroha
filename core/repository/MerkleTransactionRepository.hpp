#ifndef CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_

#include <string>
#include <msgpack.hpp>
#include "../domain/transactions/AbstractTransaction.hpp"
#include "../domain/consensus/ConsensusEvent.hpp"

namespace MerkleTransactionRepository {
bool commit(std::string const hash, ConsensusEvent const tx);
AbstractTransaction find(std::string const hash);
};

#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
