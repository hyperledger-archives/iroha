#ifndef CORE_CONSENSUS_CONSENSUSEVENT_HPP_
#define CORE_CONSENSUS_CONSENSUSEVENT_HPP_

#include <string>
#include <functional>

namespace ConsensusEvent {
class ConsensusEvent {
    void addSignature(std::string const signature);
}   
};  // namespace ConsensusEvent

#endif  // CORE_CONSENSUS_CONSENSUSEVENT_HPP_
