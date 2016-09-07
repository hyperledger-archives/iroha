#include "../util/logger.hpp"
#include "../db/ConsensusRepository.hpp"
#include "../peer/connection.hpp"

#include "../crypto/hash.hpp"

namespace sumeragi{

  struct Context{
    int numberOfPeers; //peerの数 // TODO: get this from membership service
    std::string name; //name Options
    bool isLeader;
    int myPeerNumber;
    std::string leaderNumber;
    int timeCounter;
    int peerCounter;
    std::unique_ptr<ConsensusRepository> repository;
  };
  void initialize_sumeragi(int myNumber, int aNumberOfPeer, int leaderNumber){
    
  }

  void loopMember(){

  }

  void loopLeader(){

  }

  void loop(){

  }
};
