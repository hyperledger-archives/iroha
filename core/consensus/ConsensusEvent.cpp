#include "ConsensusEvent.hpp"

#include "../domain/transactions/AbstractTransaction.hpp"

namespace ConsensusEvent {
class ConsensusEvent {
    AbstractTransaction tx;
    std::vector<std::string> txSignatures;
    std::string merkleRoot;
    std::vector<std::string> merkleRootSignatures;
}
    
}  // namespace ConsensusEvent
