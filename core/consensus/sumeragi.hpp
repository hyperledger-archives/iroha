#ifndef CORE_CONSENSUS_SUMERAGI_HPP_
#define CORE_CONSENSUS_SUMERAGI_HPP_

namespace Sumeragi {
  void initialize_sumeragi(int myNumber, int aNumberOfPeer, int leaderNumber, int batchSize = 1);
  void loop();
  void loopMember(td::shared_ptr<std::string> tx, int const currLeader);
  void loopLeader(td::shared_ptr<std::string> tx);
  void execute();
};

#endif  // CORE_CONSENSUS_SUMERAGI_HPP_
