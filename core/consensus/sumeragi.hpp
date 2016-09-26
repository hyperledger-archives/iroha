#ifndef CORE_CONSENSUS_SUMERAGI_HPP_
#define CORE_CONSENSUS_SUMERAGI_HPP_

#define COMPARATOR(code) [](auto && l, auto && r) -> bool { return code ; }

#include <vector>
#include <memory>

#include "consensus_event.hpp"

namespace sumeragi {
    void initializeSumeragi(std::vector<Node> peers);
    void loop();
    void processTransaction(const std::shared_ptr<ConsensusEvent::ConsensusEvent> event, const std::vector<Node> nodeOrder);
    void panic(const std::shared_ptr<ConsensusEvent::ConsensusEvent> event);
    void setAwkTimer(const int sleepMillisecs, const std::function<void(void)> action);
    std::vector<Node> determineConsensusOrder(const std::shared_ptr<ConsensusEvent::ConsensusEvent> event);
};  // namespace sumeragi

#endif  // CORE_CONSENSUS_SUMERAGI_HPP_
