#ifndef CORE_CONSENSUS_CONSENSUSEVENT_HPP_
#define CORE_CONSENSUS_CONSENSUSEVENT_HPP_

#include <string>
#include <functional>

#include "../domain/transactions/abstract_transaction.hpp"

namespace ConsensusEvent {
struct ConsensusEvent {
    std::unique_ptr<abstract_transaction::AbstractTransaction> tx;
    std::vector<std::string> txSignatures;
    std::string merkleRoot;
    std::vector<std::string> merkleRootSignatures;
    void addSignature(std::string const signature);
};
};  // namespace ConsensusEvent

#endif  // CORE_CONSENSUS_CONSENSUSEVENT_HPP_
