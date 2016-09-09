#include "sumeragi.hpp"

#include "../util/logger.hpp"
#include "../db/ConsensusRepository.hpp"
#include "../peer/connection.hpp"
#include "../crypto/hash.hpp"

namespace sumeragi{

struct Context{
    int numberOfPeers;  // peerの数 // TODO: get this from membership service
    std::string name;  // name Options
    bool isLeader;
    int batchSize;
    int myPeerNumber;
    std::string leaderNumber;
    int timeCounter;
    int peerCounter;
    std::unique_ptr<ConsensusRepository> repository;
    std::unique_ptr<TransactionCache> txCache;
};

void initialize_sumeragi(int myNumber, int aNumberOfPeer, int leaderNumber, int batchSize) {
    logger("initialize_sumeragi, my number:"+std::to_string(myNumber)+" leader:"+std::to_string(myNumber == leaderNumber)+"");
    context->batchSize = batchSize;
    context->myPeerNumber = myNumber;
    context->numberOfPeers = aNumberOfPeer;
    context->isLeader = myNumber == leaderNumber;
    context->leaderNumber = std::to_string(leaderNumber);
    context->timeCounter = 0;
    context->peerCounter = 0;
    context->repository = std::make_unique<ConsensusRepository>();

    buffer = "";
}

void loopLeader(td::shared_ptr<std::string> tx) {
}

void loopMember(td::shared_ptr<std::string> tx, int const currLeader) {
}

void loop() {
    logger("start loop");
    int count = 0;
    while (true) {  // TODO(M->M): replace with callback function
        if (context->txCache::hasTx()) {
            std::shared_ptr<Transaction> const tx = context->repository->popTx();
        }

        context->timeCounter++;

        int const currLeader = context->numberOfPeers;
        if (tx->hash % currLeader == context->myPeerNumber) {
            loopLeader(tx);
        } else {
            loopMember(tx, currLeader);
        }
    }
}

};  // namespace sumeragi
