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
      logger( "initialize_sumeragi my number:"+std::to_string( myNumber )+" leader:"+std::to_string(myNumber == leaderNumber)+"");
      context->myPeerNumber = myNumber;
      context->numberOfPeer = aNumberOfPeer;
      context->isLeader = myNumber == leaderNumber;
      context->leaderNumber = std::to_string(leaderNumber);
      context->timeCounter = 0;
      context->peerCounter = 0;
      context->repository = std::make_unique<ConsensusRepository>();
      buffer = "";
  }

  void loopMember(){

  }

  void loopLeader(){

  }

  void loop(){
     logger( "start loop" );
     int count = 0;
     while(true){

        context->timeCounter++;

        if(context->isLeader){
          loopLeader();
        }else{
          loopMember();
        }
      }
  }
};
