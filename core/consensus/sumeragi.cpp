#include "sumeragi.hpp"

#include "../util/logger.hpp"
#include "../domain/entityRepository.hpp"
#include "../peer/connection.hpp"
#include "../crypto/hash.hpp"

/**
* Bchain
*/
namespace sumeragi {

struct Context {
    int numberOfPeers;  // peerの数 // TODO: get this from membership service
    int maxFaulty;  // f
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

void initializeSumeragi(int const myNumber, int const numberOfPeers, int const leaderNumber, int const batchSize) {
    logger("initialize_sumeragi, my number:"+std::to_string(myNumber)+" leader:"+std::to_string(myNumber == leaderNumber)+"");
    context->batchSize = batchSize;
    context->myPeerNumber = myNumber;
    context->numberOfPeers = numberOfPeers; // TODO(M→I): これは大丈夫？
    context->maxFaulty = numberOfPeers/3;  // Default to approx. 1/3 of the network. TODO(M→M): make this configurable 
    context->validatingPeers;
    context->isLeader = myNumber == leaderNumber;
    context->leaderNumber = std::to_string(leaderNumber);
    context->timeCounter = 0;
    context->peerCounter = 0;
    context->repository = std::make_unique<ConsensusRepository>();

    buffer = "";
}

void loopLeader(td::shared_ptr<std::string> const tx) {
    ::validateTx(tx);

    seq++;

    context->processingOrder = ::determineProcessingOrder();
    ::prepareTransaction();
    
    ::addSignature();
    ::broadcastToNextValidator(tx);
    ::setAwkTimer();
}

void setAwkTimer(int const sleepMillisecs) {
    std::thread([sleepMillisecs]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillisecs));
        checkforAwk();
    }).detach();
}

void loopMember(td::shared_ptr<std::string> const tx, int const currLeader) {
    if (tx::isChain()) {
        ::validateTx(tx);
        ::addSignature();
        ::broadcastToNextValidator(tx);
        setAwkTimer(5000);

    } else if (tx::isAwk()) {


    } else if (tx::isPanic()) {

    }
}

void loopProxyTail(td::shared_ptr<std::string> const tx, int const currLeader) {
    if (tx::isChain()) {
        ::validateTx(tx);
        ::addSignature();
        ::broadcastFinalized(tx);

    } else if (tx::isPanic()) {
        if (panicMessages >= 2f) {
            ::broadcastPanic();
        }
    }
}

void loop() {
    logger("start loop");
    int count = 0;
    while (true) {  // TODO(M->M): replace with callback function
        if (context->txCache::hasTx()) {
            std::shared_ptr<Transaction> const tx = context->repository->popUnconfirmedTx();

            if (txValidator::isValid()) {
                context->timeCounter++;

                int const currLeader = context->numberOfPeers;
                if (tx->hash % Context->numberOfPeers == context->myPeerNumber) {
                    loopLeader(tx);
                } else {
                    loopMember(tx, txContext);
                }
            }
        }

        if (context->panicCache::hasPanic()) {
            std::shared_ptr<Panic> const panic = context->repository->popPanic();

            ::broadcastToPreviousPeers(panic);
        }

        Timeout.Start(5000);
        while ((TimeOut.TimeOut() == false) && (completed == false))
        {
        completed = WorkToDo()
        }
    }
}

};  // namespace sumeragi
