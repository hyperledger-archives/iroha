#include "sumeragi.hpp"
#include <queue>
#include <map>
#include <tuple>
#include <thread>
#include <algorithm>

#include "../util/logger.hpp"
#include "../repository/consensus/merkle_transaction_repository.hpp"
#include "../model/transactions/abstract_transaction.hpp"
#include "../crypto/hash.hpp"
#include "../crypto/signature.hpp"

#include "../validation/transaction_validator.hpp"
#include "../service/peer_service.hpp"
#include "./connection/connection.hpp"


#include "../service/peer_service.hpp"

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
namespace sumeragi {

    using ConsensusEvent = consensus_event::ConsensusEvent;

struct Context {
    int maxFaulty;  // f
    int proxyTailNdx;
    std::string myPublicKey;
    int panicCount;
    int numValidatingPeers;
    std::vector<peer::Node> validatingPeers;

    //std::unique_ptr<TransactionCache> txCache;
    //std::unique_ptr<TransactionValidator> txValidator;
    
    std::queue<ConsensusEvent> eventCache;

    std::map<std::string, std::shared_ptr<ConsensusEvent> > processedCache;    
};

std::unique_ptr<Context> context;

void initializeSumeragi(std::string myPublicKey, std::vector<peer::Node> peers) {
    logger::info( "initialize sumeragi", "initialize.....");
    context->validatingPeers = std::move(peers);
    context->numValidatingPeers = peers.size();
    context->maxFaulty = context->numValidatingPeers / 3;  // Default to approx. 1/3 of the network. TODO: make this configurable
    context->proxyTailNdx = context->maxFaulty*2 + 1;      
    context->panicCount = 0;

    context->myPublicKey = myPublicKey;
    logger::info( "initialize sumeragi", "complate!");
}

void processTransaction(std::shared_ptr<ConsensusEvent> const event, std::vector<peer::Node> const nodeOrder) {
    if (!transaction_validator::isValid(*event->tx)) {
        return; //TODO-futurework: give bad trust rating to nodes that sent an invalid event
    }

    event->addSignature(signature::sign( event->getHash(), peer::getMyPublicKey(), peer::getPrivateKey()));
    if (nodeOrder[context->proxyTailNdx].getPublicKey() == context->myPublicKey) {
        connection::send(nodeOrder[context->proxyTailNdx].getIP(), event->getHash()); //TIP
    } else {
        connection::sendAll(event->getHash());// TIP
    }

    setAwkTimer(5000, [&](){ 
        if (
            context->processedCache.find(event->getHash()) !=
            context->processedCache.end()
        ) { panic(event); } });

    context->processedCache[event->getHash()] = event;
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
    context->panicCount++; // TODO: reset this later
    int broadcastStart = 2*context->maxFaulty + 1 + context->maxFaulty*context->panicCount;
    int broadcastEnd = broadcastStart + context->maxFaulty;

    // Do some bounds checking
    if (broadcastStart > context->numValidatingPeers - 1) {
        broadcastStart = context->numValidatingPeers - 1;
    }

    if (broadcastEnd > context->numValidatingPeers - 1) {
        broadcastEnd = context->numValidatingPeers - 1;
    }

    connection::sendAll(event->getHash()); //TODO: change this to only broadcast to peer range between broadcastStart and broadcastEnd  
}

void setAwkTimer(int const sleepMillisecs, std::function<void(void)> const action) {
    std::thread([action, sleepMillisecs]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillisecs));
        action();
    }).detach();
}

// WIP
long long int getDistance(std::string publicKey, std::string txHash){
    // I want (node->publicKey && 0xffffff) - (txHash && 0xffffff)
    return 0;
}

std::vector<peer::Node> determineConsensusOrder(std::shared_ptr<ConsensusEvent> event/*, std::vector<double> trustVector*/) {
    std::string txHash = event->getHash();
    std::vector<std::tuple<
        peer::Node, long long int
    > > distances;

    for (int ndx = 0; ndx < context->numValidatingPeers; ++ndx) {
        auto const node = context->validatingPeers[ndx];
        // WIP
        long long int distance = getDistance(node.getPublicKey(), txHash);/* + trustVector[ndx]*/;
        
        distances.push_back(std::tuple<
            peer::Node, long long int
        >(node, distance));
    }

    std::sort(distances.begin(), distances.end(), 
        [](
            std::tuple<
                peer::Node, long long int
            > const &lhs,
            std::tuple<
                peer::Node, long long int
            > const &rhs) {
            return std::get<1>(lhs) < std::get<1>(lhs);
        }
    );
    std::vector<peer::Node> res;
    for(const auto t : distances){
        res.push_back(std::get<0>(t));
    }
    return res;
}

void loop() {
    logger::info("sumeragi","start loop");
    while (true) {  // TODO(M->M): replace with callback linking aeron
        if (context->eventCache.empty()) { //TODO: mutex here?
            const std::shared_ptr<ConsensusEvent> event = std::shared_ptr<ConsensusEvent>(&context->eventCache.front());
            if (!transaction_validator::isValid(*event->tx)) {
                continue;
            }
            // Determine node order
            std::vector<peer::Node> const nodeOrder = determineConsensusOrder(event);

            // Process transaction
            processTransaction(event, nodeOrder);
        }
            
        for (auto const &kv : context->processedCache) {
            auto event = kv.second;

            // Check if we have at least 2f+1 signatures
            if (event->txSignatures.size() > context->maxFaulty*2 + 1) {
                // Check Merkle roots to see if match for new state
                //TODO: std::vector<std::string>>const merkleSignatures = event.merkleRootSignatures;
                //TODO: try applying transaction locally and compute the merkle root
                //TODO: see if the merkle root matches or not

                // Commit locally
                merkle_transaction_repository::commit(event->getHash(), event); //TODO: add error handling in case not saved
            }
        }
    }
}

};  // namespace sumeragi
