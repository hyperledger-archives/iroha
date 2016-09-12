#include "../util/Logger.hpp"
#include "../db/ConsensusRepository.hpp"
#include "../peer/Connection.hpp"

#include "../crypto/Hash.hpp"

namespace PBFTSieve {

  struct Context {
    int numberOfPeer; //peerの数 // TODO: get this from membership service
    std::string name; //name Options
    bool isLeader;
    int myPeerNumber;
    std::string leaderNumber;
    int timeCounter;
    int peerCounter;
    std::unique_ptr<ConsensusRepository> repository;
  };

  static const int TIMEOUT = 100;

  static const std::string ACCEPT = "ACCEPT";
  static const std::string ABORT  = "ABORT";

  std::unique_ptr<Context> context;
  std::string buffer;

  enum Status{
    S701,
    S702,
    S703,
    S704,
    S705,
    S706,
    S707,
    S708,
    S709,
    S710,
    S711,
    S712,
    S713,
    S714,
  } status;

  bool validate(std::string message){
    logger( "validate("+message+")" );
    return true;
  }

  std::string execute(std::string message){
    return hash::sha3_256_hex(message);
  }

  void initialize_pbft_sieve(int myNumber, int aNumberOfPeer, int leaderNumber){
    logger( "initialize_pbft_sieve my number:"+std::to_string( myNumber )+" leader:"+std::to_string(myNumber == leaderNumber)+"");
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
    std::shared_ptr<std::string> tx;
    switch (status) {
      case Status::S701: //一度LocalのTxデータをLeaderに送る
        logger("[Member] S701");
        tx = context->repository->findTemporaryAllTxData();
        Connection::send(context->leaderNumber, *tx);
        status = Status::S704;
        break;
      case Status::S704: //Leaderから送られたデータのHashを出し、Leaderに送りかえす
        logger("[Member] S704");
        Connection::receive([](std::string from, std::string message){
          if(from == context->leaderNumber){
            std::string execResult = execute(message);
            Connection::send(context->leaderNumber, execResult);
            status = Status::S710;
          }
        });
        break;
      case Status::S710: // リーダーから確定が来たらデータを保存する
        logger("[Member] S710");
        Connection::receive([](std::string from, std::string message){
          if(from == context->leaderNumber){
            if(message == ACCEPT){
              // Temporaryのデータを確定する。
              context->repository->confirmTx();
              status = Status::S711;
            }
          }
        });
        break;
      case Status::S711:
        if(context->timeCounter > TIMEOUT){
          status = Status::S713;
          context->timeCounter = 0;
        }else{
          status = Status::S704;
        }
        break;
      case Status::S713:
        context->repository->addBlock();
        status = Status::S701;
      default:
        logger("Error!!");
        exit(1);
    }
  }

  void loopLeader(){
    switch(status){
    case Status::S701:
      context->peerCounter = 0;
      do{
        Connection::receive([](std::string from, std::string message){
          if(validate(message)){
            buffer += message;
            context->peerCounter++;
          }
        });
      }while(context->peerCounter < context->numberOfPeer);
      status = Status::S703;
      break;
    case Status::S703:
      Connection::sendAll(buffer);
      buffer = "";
      status = Status::S705;
      break;
    case Status::S705:
      context->peerCounter = 0;
      // ToDo timeout

      do{
        Connection::receive([](std::string from, std::string message){
          context->peerCounter++;
        });
        if(context->timeCounter > TIMEOUT/* Time out */){
          status = Status::S707;
        }
      }while(context->peerCounter < context->numberOfPeer/2+1);
      status = Status::S706;
      break;
    case Status::S706:
      // ToDo check results;
      if(true){
        status = Status::S708;
      }else{
        status = Status::S707;
      }
      break;
    case Status::S707:
      Connection::sendAll(ABORT/*Abort message*/);
      status = Status::S710;
      break;
    case Status::S708:
      Connection::sendAll(ACCEPT/*Accept message*/);
      status = Status::S710;
      break;
    }
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
