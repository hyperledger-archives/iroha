#ifndef CORE_CONSENSUS_SUMERAGI_HPP_
#define CORE_CONSENSUS_SUMERAGI_HPP_

#define COMPARATOR(code) [](auto && l, auto && r) -> bool { return code ; }

namespace sumeragi {
void initializeSumeragi(int myNumber, int aNumberOfPeer, int leaderNumber, int batchSize = 1);
void loop();
void processTransaction(td::shared_ptr<ConsensusEvent> const event, std::vector<Node> const nodeOrder);
void panic(std::shared_ptr<ConsensusEvent> const event);
void setAwkTimer(int const sleepMillisecs, std::function<void(void)> const action, actionArgs ...);
std::vector<Node> determineConsensusOrder(std::shared_ptr<ConsensusEvent> const event);
};  // namespace sumeragi

#endif  // CORE_CONSENSUS_SUMERAGI_HPP_
