#ifndef CORE_CONSENSUS_CONSENSUSEVENT_HPP_
#define CORE_CONSENSUS_CONSENSUSEVENT_HPP_

#include <string>
#include <functional>
#include <memory>

#include "../model/transactions/abstract_transaction.hpp"

namespace consensus_event {

struct ConsensusEvent {
    std::unique_ptr<abstract_transaction::AbstractTransaction> tx;
    std::vector<std::string> txSignatures;
    std::string merkleRoot;
    std::vector<std::string> merkleRootSignatures;

    void addSignature(const std::string& signature);
    std::string getHash() const;
};
};  // namespace ConsensusEvent

#endif  // CORE_CONSENSUS_CONSENSUSEVENT_HPP_
