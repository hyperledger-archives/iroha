#include "Sumeragi.hpp"

#include "../util/Logger.hpp"
#include "../repository/TransactionRepository.hpp"
#include "../domain/AbstractTransaction.hpp"
#include "../peer/Connection.hpp"
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
    int maxFaulty;  // f
    const unsigned char* myPublicKey;
    int panicCount;
    std::unique_ptr<TransactionRepository> repository;
    std::unique_ptr<TransactionCache> txCache;
    std::unique_ptr<TransactionValidator> txValidator;
    std::queue<ConsensusEvent> eventCache;
};

void initializeSumeragi(int const myNumber, int const numberOfPeers, int const leaderNumber, int const batchSize) {
    logger("initialize_sumeragi, my number:"+std::to_string(myNumber)+" leader:"+std::to_string(myNumber == leaderNumber)+"");
    context->maxFaulty = numberOfPeers/3;  // Default to approx. 1/3 of the network. TODO(M→M): make this configurable 
    context->txRepository = std::make_unique<TransactionRepository>();
}

void processTransaction(td::shared_ptr<ConsensusEvent> const event, std::vector<Node> const nodeOrder) {
    if (!txValidator::isValid(event)) {
        return; //TODO-futurework: give bad trust rating to nodes that sent an invalid event
    }

    event::addSignature(sign(hash)); //TODO
    if (!context->isProxyTail) {}
        peerConnection::broadcastToProxyTail(awk); //TODO
    } else {
        peerConnection::broadcastFinalizedAll(event); //TODO
    }

    setAwkTimer(5000, [&]{ if (unconfirmed(event)) {panic();} };);
}

/**
* 
* For example, given:
* if f := 1, then
*  _________________    _________________
* /        A        \  /        B        \
* |---|  |---|  |---|  |---|  |---|  |---|
* | 0 |--| 1 |--| 2 |--| 3 |--| 4 |--| 5 |
* |---|  |---|  |---|  |---|  |---|  |---|,
*
* if 2f+1 signature are not received within the timer's limit, then
* the set of considered validators, A, is expanded by f (e.g., by 1 in the example below):
*  ________________________    __________
* /           A            \  /    B     \ 
* |---|  |---|  |---|  |---|  |---|  |---|
* | 0 |--| 1 |--| 4 |--| 5 |--| 2 |--| 3 |
* |---|  |---|  |---|  |---|  |---|  |---|.
*/
void panic(std::shared_ptr<ConsensusEvent> const event) {
    context->panicCount++;
    int broadcastStart = 2*context->maxFaulty + 1 + context->maxFaulty*context->panicCount;
    int broadcastEnd = broadcastStart + context->maxFaulty;
    peerConnection::broadcastToPeerRange(event, broadcastStart, broadcastEnd); //TODO
}

void setAwkTimer(int const sleepMillisecs, std::function<void(void)> action, actionArgs ...) {
    std::thread([sleepMillisecs, tx]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillisecs));
        action();
    }).detach();
}

std::vector<Node> determineConsensusOrder(std::shared_ptr<ConsensusEvent> const event, std::vector<double> trustVector) {
    unsigned char* const publicKey = event->publicKey;
    std::vector<std::tuple> distances = std::make_shared();

    for (int ndx = 0; ndx < context->membership::nodes::size; ++ndx) {
        auto node = membership::nodes[ndx];
        long long int distance = (*nodeKey->get() && 0xffffff) - (publicKey && 0xffffff) + trustVector[ndx];
        
        distances[ndx] = std::make_tuple(publicKey, distance);
    }

    std::vector<Node> nodeOrder = std::sort(distances.begin(), distances.end(), COMPARATOR(l::get<1> < r::get<1>));
    
    return nodeOrder;
}

void loop() {
    logger("start loop");
    while (true) {  // TODO(M->M): replace with callback linking aeron
        if (context->eventCache::hasConsensusEvent()) { //TODO: mutex here?
            std::shared_ptr<ConsensusEvent> const event = context->eventCache::pop();
            if (!context->consensusEventValidator.isValid(event)) {
                continue;
            }
            // Determine node order
            std::vector<Node> nodeOrder = determineConsensusOrder();

            // Process transaction
            processTransaction(event, nodeOrder);

            if (awkCache->signatures::size() > context->maxFaulty*2 + 1) { // TODO check syntax
                // Commit locally
                transactionRepository->commitTransaction(event); //TODO: add error handling in case not saved
            }
        }
    }
}

};  // namespace Sumeragi
