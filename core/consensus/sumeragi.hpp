#ifndef CORE_CONSENSUS_SUMERAGI_HPP_
#define CORE_CONSENSUS_SUMERAGI_HPP_

#define COMPARATOR(code) [](auto && l, auto && r) -> bool { return code ; }

namespace Sumeragi {
  void initializeSumeragi(int myNumber, int aNumberOfPeer, int leaderNumber, int batchSize = 1);
  void loop();
  void processTransaction(td::shared_ptr<ConsensusEvent> const event, std::vector<Node> const nodeOrder);
};

#endif  // CORE_CONSENSUS_SUMERAGI_HPP_
