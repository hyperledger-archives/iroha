#ifndef CORE_CONSENSUS_SUMERAGI_HPP_
#define CORE_CONSENSUS_SUMERAGI_HPP_

#define COMPARATOR(code) [](auto && l, auto && r) -> bool { return code ; }

#include <vector>
#include <memory>

#include "consensus_event.hpp"

#include "../service/peer_service.hpp"


namespace sumeragi {
    void initializeSumeragi(std::string myPublicKey, std::vector<peer::Node> peers);
    void loop();
    void processTransaction(const std::shared_ptr<consensus_event::ConsensusEvent> event, const std::vector<peer::Node> nodeOrder);
    void panic(const std::shared_ptr<consensus_event::ConsensusEvent> event);
    void setAwkTimer(const int sleepMillisecs, const std::function<void(void)> action);
    std::vector<peer::Node> determineConsensusOrder(const std::shared_ptr<consensus_event::ConsensusEvent> event);
};  // namespace sumeragi

#endif  // CORE_CONSENSUS_SUMERAGI_HPP_
