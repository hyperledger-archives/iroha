#include "sumeragi.hpp"

#include "../util/logger.hpp"
#include "../repository/merkle_transaction_repository.hpp"
#include "../domain/transactions/abstract_transaction.hpp"
#include "../connection/connection.hpp"
#include "../crypto/hash.hpp"
#include "../validation/transaction_validator.hpp"
#include "../service/peer_service.hpp"
#include "./connection/connection.hpp"

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

struct Context {
    int maxFaulty;  // f
    int proxyTailNdx;
    const unsigned char* myPublicKey;
    int panicCount;
    int numValidatingPeers;
    std::vector<Node> validatingPeers;
    std::unique_ptr<merkle_transaction_repository> txRepository;
    std::unique_ptr<TransactionCache> txCache;
    std::unique_ptr<TransactionValidator> txValidator;
    std::queue<ConsensusEvent> eventCache;

    std::map<ConsensusEvent> processedCache;
    connection conn;
};

std::unique_ptr<Context> context;

void initializeSumeragi(std::string myPublicKey, std::vector<Node> peers) {
    logger::info( __FILE__, "initializeSumeragi");
    context->validatingPeers = peers;
    context->numValidatingPeers = validatingPeers::size();
    context->maxFaulty = context->numValidatingPeers / 3;  // Default to approx. 1/3 of the network. TODO: make this configurable
    context->proxyTailNdx = context->maxFault*2 + 1;  
    context->txRepository = std::make_unique<merkle_transaction_repository>();
    context->panicCount = 0;
    context->conn = std::make_unique<connection>(); //TODO: is this syntax correct (connection is a namespace...)?

    context->eventCache = std::make_uniquestd::queue<ConsensusEvent>>();
    context->processedCache = std::make_unique<std::map<ConsensusEvent>>();

    context->myPublicKey = myPublicKey;
}

void processTransaction(td::shared_ptr<ConsensusEvent> const event, std::vector<Node> const nodeOrder) {
    if (!txValidator::isValid(event)) {
        return; //TODO-futurework: give bad trust rating to nodes that sent an invalid event
    }

    event::addSignature(sign(hash));
    if (nodeOrder::get(context->proxyTailNdx)->publicKey == context->myPublicKey) {
        context->conn::send(nodeOrder::get(proxyTail)::getIP(), event);
    } else {
        context->conn::sendAll(event);
    }

    setAwkTimer(5000, [&]{ if (context->processedCache::count(event::getHash()) > 0) { panic(); } });

    context->processedCache[event::getHash()] = event;
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

    context->conn.sendAll(event); //TODO: change this to only broadcast to peer range between broadcastStart and broadcastEnd  
}

void setAwkTimer(int const sleepMillisecs, std::function<void(void)> const action) {
    std::thread([sleepMillisecs, tx]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillisecs));
        action();
    }).detach();
}

std::vector<Node> determineConsensusOrder(std::shared_ptr<ConsensusEvent> const event/*, std::vector<double> trustVector*/) {
    unsigned char* const txHash = event::getHash();
    std::vector<std::tuple> distances = std::make_shared<std::vector<std::tuple>>();

    for (int ndx = 0; ndx < context->numValidatingPeers; ++ndx) {
        auto const node = context->validatingPeers::get(ndx);
        long long int distance = (node->publicKey && 0xffffff) - (txHash && 0xffffff)/* + trustVector[ndx]*/;
        
        distances[ndx] = std::make_tuple(node->publicKey, distance);
    }

    std::vector<Node> const nodeOrder = std::sort(distances.begin(), distances.end(), COMPARATOR(l::get<1> < r::get<1>));
    
    return nodeOrder;
}

void loop() {
    logger("start loop");
    while (true) {  // TODO(M->M): replace with callback linking aeron
        if (context->eventCache::empty()) { //TODO: mutex here?
            std::shared_ptr<ConsensusEvent> const event = context->eventCache::pop();
            if (!context->consensusEventValidator.isValid(event)) {
                continue;
            }
            // Determine node order
            std::vector<Node> const nodeOrder = determineConsensusOrder(event);

            // Process transaction
            processTransaction(event, nodeOrder);
        }
            
        for (auto const &key : context->processedCache) {
            auto event = context->processedCache[&key];

            // Check if we have at least 2f+1 signatures
            if (event::getSignatures::size() > context->maxFaulty*2 + 1) {
                // Check Merkle roots to see if match for new state
                //TODO: std::vector<std::string>>const merkleSignatures = event.merkleRootSignatures;
                //TODO: try applying transaction locally and compute the merkle root
                //TODO: see if the merkle root matches or not

                // Commit locally
                transactionRepository->commitTransaction(event); //TODO: add error handling in case not saved
            }
        }
    }
}

};  // namespace sumeragi
