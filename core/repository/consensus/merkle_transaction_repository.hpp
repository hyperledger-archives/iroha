#ifndef CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_

#include <string>
#include <memory>
#include "../../model/transactions/abstract_transaction.hpp"

#include "../../consensus/consensus_event.hpp"

namespace merkle_transaction_repository {
bool commit(std::string const hash, consensus_event::ConsensusEvent const tx);
std::unique_ptr<abstract_transaction::AbstractTransaction> findLeaf(std::string const hash);
};  // namespace merkle_transaction_repository

#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
