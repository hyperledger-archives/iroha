#include "sumeragi.hpp"

#include "../util/logger.hpp"
#include "../db/ConsensusRepository.hpp"
#include "../peer/connection.hpp"
#include "../crypto/hash.hpp"

namespace sumeragi{

struct Context {
    int numberOfPeers;  // peerの数 // TODO: get this from membership service
    std::string name;  // name Options
    bool isLeader;
    bool isPanic;
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
    seq++;

    ::broadcastAllValidators(tx);
    ::replyClient(viewNumber, localTimestamp, seq, client);
}

void loopMember(td::shared_ptr<std::string> const tx, int const currLeader) {
    if (tx::isCommit()) {
        if (match(seq)) {
            addCommitMsg();
        }

        if (numCommits == f+1) {
            ::broadcastAllValidators(commit);
            ::replyClient(viewNumber, localTimestamp, seq, client);

            if (conflictWithMyself) {
                ::broadcastAllValidators(viewChange);
            }
        } else if (numCommits = 2f) {
            complete(tx);  // Finality is achieved
        }

    } else if (tx::isPrepare()) {
        if (tx.seq == seq + 1) {
            seq = tx.seq;
            applyTx(tx);
            ::broadcastAllValidators(txCommit);
            ::replyClient(viewNumber, localTimestamp, seq, client);
        }

    } else if (tx::isPanic()) {
        if (!context->isPanic) {

        }
    }
}

void loop() {
    logger("start loop");
    int count = 0;
    while (true) {  // TODO(M->M): replace with callback function
        if (context->txCache::hasTx()) {
            std::shared_ptr<Transaction> const tx = context->repository->popTx();

            if (txValidator::isValid()) {
                context->timeCounter++;

                int const currLeader = context->numberOfPeers;
                if (tx->hash % currLeader == context->myPeerNumber) {
                    loopLeader(tx);
                } else {
                    loopMember(tx, currLeader);
                }
            }
        }

        if (context->panicCache::hasPanic()) {
            std::shared_ptr<Panic> const panic = context->repository->popPanic();

            if (panic.isFromClient()){
                ::broadcastAllValidators(panic);
            } else {
                
            }
        }
    }
}

};  // namespace sumeragi
