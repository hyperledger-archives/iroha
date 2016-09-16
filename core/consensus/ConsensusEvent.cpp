#include "ConsensusEvent.hpp"

#include "../domain/transactions/AbstractTransaction.hpp"

namespace ConsensusEvent {
class ConsensusEvent {
    AbstractTransaction tx;
    std::vector<std::string> signatures;
}
    
}  // namespace ConsensusEvent
