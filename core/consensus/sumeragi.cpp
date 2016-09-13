#include "Sumeragi.hpp"

#include "../util/Logger.hpp"
#include "../domain/EntityRepository.hpp"
#include "../domain/AbstractTransaction.hpp"
#include "../peer/Connection.hpp" // TODO: rather than this low-level interface, abstract out events and broadcasts
#include "../crypto/Hash.hpp"
#include "../validation/TransactionValidator.hpp"

/**
* |ーーー|　|ーーー|　|ーーー|　|ーーー|
* |　ス　|ー|　メ　|ー|　ラ　|ー|　ギ　|
* |ーーー|　|ーーー|　|ーーー|　|ーーー|
*
* A chain-based byzantine fault tolerant consensus algorithm, based in large part on BChain: 
*
* Duan, S., Meling, H., Peisert, S., & Zhang, H. (2014). Bchain: Byzantine replication with 
* high throughput and embedded reconfiguration. In International Conference on Principles of 
* Distributed Systems (pp. 91-106). Springer.
*/
namespace Sumeragi {

struct Context {
    int numberOfPeers;  // peerの数 // TODO: get this from membership service
    int maxFaulty;  // f
    std::string name;  // name Options
    bool isLeader;
    int myPeerNumber;
    std::string leaderNumber;
    int timeCounter;
    int peerCounter;
    std::unique_ptr<TransactionRepository> repository;
    std::unique_ptr<TransactionCache> txCache;
    std::unique_ptr<TransactionValidator> txValidator;
    std::queue<ConsensusEvent> eventCache;//TODO: explore deque
    std::map viewChangeCache;
    std::map awkCache;
};

void initializeSumeragi(int const myNumber, int const numberOfPeers, int const leaderNumber, int const batchSize) {
    logger("initialize_sumeragi, my number:"+std::to_string(myNumber)+" leader:"+std::to_string(myNumber == leaderNumber)+"");
    context->maxFaulty = numberOfPeers/3;  // Default to approx. 1/3 of the network. TODO(M→M): make this configurable 
    context->txRepository = std::make_unique<TransactionRepository>();

    buffer = "";
}

void processTransaction(td::shared_ptr<std::string> const tx) {
    if (!txValidator::isValid(tx)) {
        return;
    }

    tx::addSignature(); //TODO
    peerConnection::broadcastToNextValidator(tx); //TODO
    if (isProxyTail) {
        loopProxyTail();
    }

    setAwkTimer(5000, [&]{ reconfigure(); };);
}

// /**
// * Move the suspected validator to the end of the chain and the suspector to the 2f+1'th position.
// * 
// * For example, given:
// * |---|  |---|  |---|  |---|  |---|  |---|
// * | H |--| 1 |--| 2 |--| 3 |--| 4 |--| 5 |
// * |---|  |---|  |---|  |---|  |---|  |---|,
// *
// * if [2] suspects [3] and f := 2, then the validation chain order will become:
// * |---|  |---|  |---|  |---|  |---|  |---|
// * | H |--| 1 |--| 4 |--| 5 |--| 2 |--| 3 |
// * |---|  |---|  |---|  |---|  |---|  |---|.
// *
// * Only the head (H) can reconfigure the chain order. If the head has a problem, other nodes should 
// * request a view change.
// */
// void reconfigureSuspects(int const suspected, int const suspector) {
// //TODO:
// }

void reconfigure() {
    // Any suspects?
    getSuspects();
    reconfigureSuspects();

    // If no suspects, kick the 2f+1 node to the end of the chain
}

void setAwkTimer(int const sleepMillisecs, std::function<void(void)> action, actionArgs ...) {
    std::thread([sleepMillisecs, tx]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillisecs));
        action();
    }).detach();
}

void loopProxyTail(td::shared_ptr<std::string> const tx, int const currLeader) {
    if (tx::isChain()) { //TODO
        context->txValidator::isValid(tx); //TODO
        tx::addSignature(); //TODO
        peerConnection::broadcastFinalizedAll(tx); //TODO

    } else if (tx::isPanic()) {
        if (panicMessages.length >= context->maxFaulty) { //TODO
            peerConnection::broadcastPanicAll(); //TODO
        }
    }
}

void loop() {
    logger("start loop");
    while (true) {  // TODO(M->M): replace with callback linking aeron
        if (context->eventCache::hasConsensusEvent()) { //TODO: mutex here?
            std::shared_ptr<ConsensusEvent> const event = context->eventCache::pop();
            if (!context->consensusEventValidator.isValid(event)) {
                continue;
            }

            if (ConsensusEvent.types.transaction == event->type) {// TODO
                // Determine node order
                std::vector<Node> nodeOrder = determineConsensusOrder(); //TODO

                // Process transaction
                bool const transactionResult = processTransaction(nodeOrder); //TODO
                if (transactionResult) {
                    peerConnection::broadcastProxyTail(awk); //TODO
                }


            } else if (ConsensusEvent::types::awk == event->type) {
                // Validate awk event
                //TODO:
                // Save the event to cache. If 2f signatures, then commit, because with yourself it is 2f+1
                awkCache.put(event); //TODO

                if (awkCache.size() > context->maxFaulty*2 + 1) {
                    // Commit locally
                    transactionRepository->commitTransaction(); //TODO
                }

            } else if (ConsensusEvent::types::suspicion == event->type) {
                //TODO: validate suspicion
                // Request view change
                

            } else if (ConsensusEvent::types::viewChange == event->type) {
                // Validate view change event
                //TODO:

                // Save the event to cache. If 2f + 1 then commit the view change.
                viewChangeCache.put(event); //TODO
                peerConnection::broadcastAll(viewChange); //TODO
            }
    }
}

};  // namespace Sumeragi
