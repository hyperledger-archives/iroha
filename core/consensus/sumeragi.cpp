/*
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *          http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <service/flatbuffer_service.h>
#include <crypto/base64.hpp>
#include <crypto/hash.hpp>
#include <crypto/signature.hpp>
#include <infra/config/iroha_config_with_json.hpp>
#include <infra/config/peer_service_with_json.hpp>
#include <service/executor.hpp>
#include <service/peer_service.hpp>
#include <thread_pool.hpp>
#include <util/logger.hpp>
#include <util/timer.hpp>

#include <atomic>
#include <cmath>
#include <deque>
#include <map>
#include <queue>
#include <string>
#include <thread>

#include "connection/connection.hpp"
#include "sumeragi.hpp"

/**
 * |ーーー|　|ーーー|　|ーーー|　|ーーー|
 * |　ス　|ー|　メ　|ー|　ラ　|ー|　ギ　|
 * |ーーー|　|ーーー|　|ーーー|　|ーーー|
 *
 * A chain-based byzantine fault tolerant consensus algorithm, based in large
 * part on BChain:
 *
 * Duan, S., Meling, H., Peisert, S., & Zhang, H. (2014). Bchain: Byzantine
 * replication with high throughput and embedded reconfiguration. In
 * International Conference on Principles of Distributed Systems (pp. 91-106).
 * Springer.
 */
namespace sumeragi {

using iroha::ConsensusEvent;
using iroha::Signature;
using iroha::Transaction;

std::map<std::string, std::string> txCache;

static ThreadPool pool(ThreadPoolOptions{
    .threads_count =
        config::IrohaConfigManager::getInstance().getConcurrency(0),
    .worker_queue_size =
        config::IrohaConfigManager::getInstance().getPoolWorkerQueueSize(1024),
});

namespace detail {

std::string hash(const Transaction& tx) {
  // ToDo We should make tx.to_string()
  return hash::sha3_256_hex(flatbuffer_service::toString(tx));
};

bool eventSignatureIsEmpty(const ::iroha::ConsensusEvent& event) {
  logger::info("sumeragi") << "if " << (event.peerSignatures() != nullptr);
  if (&event != nullptr && event.peerSignatures() != nullptr) {
    return event.peerSignatures()->size() == 0;
  } else {
    return 0;
  }
}

void printJudge(int numValidSignatures, int numValidationPeer, int faulty) {
  std::stringstream resLine[5];
  for (int i = 0; i < numValidationPeer; i++) {
    if (i < numValidSignatures) {
      resLine[0] << "\033[1m\033[92m+-ー-+\033[0m";
      resLine[1] << "\033[1m\033[92m| 　 |\033[0m";
      resLine[2] << "\033[1m\033[92m|-承-|\033[0m";
      resLine[3] << "\033[1m\033[92m| 　 |\033[0m";
      resLine[4] << "\033[1m\033[92m+-＝-+\033[0m";
    } else {
      resLine[0] << "\033[91m+-ー-+\033[0m";
      resLine[1] << "\033[91m| 　 |\033[0m";
      resLine[2] << "\033[91m| 否 |\033[0m";
      resLine[3] << "\033[91m| 　 |\033[0m";
      resLine[4] << "\033[91m+-＝-+\033[0m";
    }
  }
  for (int i = 0; i < 5; ++i) logger::explore("sumeragi") << resLine[i].str();

  std::string line;
  for (int i = 0; i < numValidationPeer; i++) line += "==＝==";
  logger::explore("sumeragi") << line;

  logger::explore("sumeragi")
      << "numValidSignatures:" << numValidSignatures << " faulty:" << faulty;
}

void printAgree() {
  logger::explore("sumeragi") << "\033[1m\033[92m+==ーー==+\033[0m";
  logger::explore("sumeragi") << "\033[1m\033[92m|+-ーー-+|\033[0m";
  logger::explore("sumeragi") << "\033[1m\033[92m|| 承認 ||\033[0m";
  logger::explore("sumeragi") << "\033[1m\033[92m|+-ーー-+|\033[0m";
  logger::explore("sumeragi") << "\033[1m\033[92m+==ーー==+\033[0m";
}

void printReject() {
  logger::explore("sumeragi") << "\033[91m+==ーー==+\033[0m";
  logger::explore("sumeragi") << "\033[91m|+-ーー-+|\033[0m";
  logger::explore("sumeragi") << "\033[91m|| 否認 ||\033[0m";
  logger::explore("sumeragi") << "\033[91m|+-ーー-+|\033[0m";
  logger::explore("sumeragi") << "\033[91m+==ーー==+\033[0m";
}
}  // namespace detail

struct Context {
  bool isSumeragi;          // am I the leader or am I not?
  std::uint64_t maxFaulty;  // f
  std::uint64_t proxyTailNdx;
  std::int32_t panicCount;
  std::int64_t commitedCount = 0;
  std::uint64_t numValidatingPeers;
  std::string myPublicKey;
  std::deque<std::unique_ptr<peer::Node>> validatingPeers;

  Context() { update(); }

  Context(std::vector<std::unique_ptr<peer::Node>>&& peers) {
    for (auto&& p : peers) {
      validatingPeers.push_back(std::move(p));
    }
  }

  void update() {
    auto peers = config::PeerServiceConfig::getInstance().getGroup();
    for (const auto& p : peers) {
      validatingPeers.push_back(std::make_unique<peer::Node>(
          p["ip"].get<std::string>(), p["publicKey"].get<std::string>()));
      logger::info("sumeragi")
          << "Add " << p["ip"].get<std::string>() << " to peerList";
    }

    this->numValidatingPeers = this->validatingPeers.size();
    // maxFaulty = Default to approx. 1/3 of the network.
    this->maxFaulty =
        config::IrohaConfigManager::getInstance().getMaxFaultyPeers(
            this->numValidatingPeers / 3);
    this->proxyTailNdx = this->maxFaulty * 2 + 1;

    if (this->validatingPeers.empty()) {
      logger::error("sumeragi") << "could not find any validating peers.";
      exit(EXIT_FAILURE);
    }
    logger::info("sumeragi") << "peerList is not empty";

    if (this->proxyTailNdx >= this->validatingPeers.size()) {
      this->proxyTailNdx = this->validatingPeers.size() - 1;
    }

    this->panicCount = 0;
    this->myPublicKey =
        config::PeerServiceConfig::getInstance().getMyPublicKeyWithDefault(
            "Invalied");

    this->isSumeragi =
        this->validatingPeers.at(0)->publicKey == this->myPublicKey;
    logger::info("sumeragi") << "update finished";
  }
};

std::unique_ptr<Context> context = nullptr;

void initializeSumeragi() {
  logger::explore("sumeragi") << "\033[95m+==ーーーーーーーーー==+\033[0m";
  logger::explore("sumeragi") << "\033[95m|+-ーーーーーーーーー-+|\033[0m";
  logger::explore("sumeragi") << "\033[95m|| 　　　　　　　　　 ||\033[0m";
  logger::explore("sumeragi") << "\033[95m|| いろは合意形成機構 ||\033[0m";
  logger::explore("sumeragi")
      << "\033[95m|| 　　　\033[1mすめらぎ\033[0m\033[95m　　 ||\033[0m";
  logger::explore("sumeragi") << "\033[95m|| 　　　　　　　　　 ||\033[0m";
  logger::explore("sumeragi") << "\033[95m|+-ーーーーーーーーー-+|\033[0m";
  logger::explore("sumeragi") << "\033[95m+==ーーーーーーーーー==+\033[0m";
  logger::explore("sumeragi") << "- 起動/setup";
  logger::explore("sumeragi") << "- 初期設定/initialize";
  // merkle_transaction_repository::initLeaf();
  /*
  logger::info("sumeragi") << "My key is "
    << config::PeerServiceConfig::getInstance()
      .getMyIpWithDefault("Invalid!!");
  */
  logger::info("sumeragi") << "Sumeragi setted";
  logger::info("sumeragi") << "set number of validatingPeer";

  context = std::make_unique<Context>();

  connection::iroha::SumeragiImpl::Torii::receive(
      [](const std::string& from, flatbuffers::unique_ptr_t&& transaction) {
        logger::info("sumeragi") << "receive!";

        flatbuffers::unique_ptr_t event = flatbuffer_service::toConsensusEvent(
            *flatbuffers::GetRoot<::iroha::Transaction>(transaction.get()));

        auto &&task = [&event]() {
          processTransaction(std::move(event));
        };
        pool.process(std::move(task));

        // ToDo I think std::unique_ptr<const T> is not popular. Is it?
        // return std::unique_ptr<ConsensusEvent>(const_cast<ConsensusEvent*>(
        //                                               flatbuffers::GetRoot<ConsensusEvent>(fbb.GetBufferPointer())));
        // send processTransaction(event) as a task to processing pool
        // this returns std::future<void> object
        // (std::future).get() method locks processing until result of
        // processTransaction will be available but processTransaction returns
        // void, so we don't have to call it and wait
        // std::function<void()> &&task = std::bind(processTransaction, event);
        // pool.process(std::move(task));
      });

  connection::iroha::SumeragiImpl::Verify::receive(
      [](const std::string& from, flatbuffers::unique_ptr_t&& eventUniqPtr) {

        auto eventPtr =
            flatbuffers::GetRoot<::iroha::ConsensusEvent>(eventUniqPtr.get());

        logger::info("sumeragi") << "receive!";  // ToDo rewrite
        logger::info("sumeragi") << "received message! sig:["
                                 << eventPtr->peerSignatures()->size() << "]";
        //        logger::info("sumeragi") << "received message! status:[" <<
        if(eventPtr->code() == iroha::Code_COMMIT) {
          if(txCache.find(
                  detail::hash(*eventPtr->transactions()->Get(0))
             ) ==
            txCache.end()
          ) {
              executor::execute(*eventPtr->transactions()->Get(0));
              txCache[detail::hash(*eventPtr->transactions()->Get(0))] = "commited";
          }
        }else {
          // send processTransaction(event) as a task to processing pool
          // this returns std::future<void> object
          // (std::future).get() method locks processing until result of
          // processTransaction will be available but processTransaction returns
          // void, so we don't have to call it and wait
          // std::function<void()>&& task =
          //    std::bind(processTransaction, std::move(event));
          // pool.process(std::move(task));

          // Copy ConsensusEvent
          auto &&task = [&eventUniqPtr]() {
              processTransaction(std::move(eventUniqPtr));
          };
          pool.process(std::move(task));
        }
      });

  logger::info("sumeragi") << "initialize numValidatingPeers :"
                           << context->numValidatingPeers;
  logger::info("sumeragi") << "initialize maxFaulty :" << context->maxFaulty;
  logger::info("sumeragi") << "initialize proxyTailNdx :"
                           << context->proxyTailNdx;

  logger::info("sumeragi") << "initialize panicCount :" << context->panicCount;
  logger::info("sumeragi") << "initialize myPublicKey :"
                           << context->myPublicKey;

  // TODO: move the peer service and ordering code to another place
  // determineConsensusOrder(); // side effect is to modify validatingPeers
  logger::info("sumeragi") << "initialize is sumeragi :"
                           << static_cast<int>(context->isSumeragi);
  logger::info("sumeragi") << "initialize.....  complete!";
}


std::uint64_t getNextOrder() {
  return 0l;
  // ToDo
  // return merkle_transaction_repository::getLastLeafOrder() + 1;
}

void processTransaction(flatbuffers::unique_ptr_t&& eventUniqPtr) {
  auto eventPtr =
      flatbuffers::GetRoot<::iroha::ConsensusEvent>(eventUniqPtr.get());

  logger::info("sumeragi") << "processTransaction";
  logger::info("sumeragi") << "valid";
  logger::info("sumeragi") << "Add my signature...";

  logger::info("sumeragi") << "tx[0] hash raw";
  for (auto e : *eventPtr->transactions()->Get(0)->hash()) {
    std::cout << (char)e;
  }
  std::cout << std::endl;

  logger::info("sumeragi") << "hash:"
    << detail::hash(*eventPtr->transactions()->Get(0));

  logger::info("sumeragi")    <<  "pub: "  <<
  config::PeerServiceConfig::getInstance().getMyPublicKeyWithDefault("AA");
  logger::info("sumeragi")    <<  "priv:"  <<
  config::PeerServiceConfig::getInstance().getMyPrivateKeyWithDefault("AA");
  logger::info("sumeragi")    <<  "sig: "  <<  signature::sign(
      // ToDo We should add it.
      detail::hash(*eventPtr->transactions()->Get(0)),
      config::PeerServiceConfig::getInstance().getMyPublicKeyWithDefault("AA"),
      config::PeerServiceConfig::getInstance().getMyPrivateKeyWithDefault("AA")
  );

  logger::info("sumeragi") << "Add basically own signature";
  if (eventPtr->peerSignatures() != nullptr) {
      logger::info("sumeragi") << "Length: " << eventPtr->peerSignatures()->size();
  }
  // This is a new event, so we should verify, sign, and broadcast it
  auto newEventUniqPtr = flatbuffer_service::addSignature(
      *eventPtr,
      config::PeerServiceConfig::getInstance().getMyPublicKeyWithDefault(
        "Invalid"
      ),  // ??
      ""
  );

  eventPtr = flatbuffers::GetRoot<::iroha::ConsensusEvent>(newEventUniqPtr.get());
  if (eventPtr->peerSignatures() != nullptr) {
    logger::info("sumeragi") << "New Length: " << eventPtr->peerSignatures()->size();
  }

  logger::info("sumeragi") << "if eventSignatureIsEmpty";

  if (detail::eventSignatureIsEmpty(*eventPtr) && context->isSumeragi) {
    logger::info("sumeragi") << "signatures.empty() isSumragi";
    // Determine the order for processing this event
    // event.set_order(getNextOrder());//TODO getNexOrder is always return 0l;
    // logger::info("sumeragi") << "new  order:" << event.order();
  } else if (!detail::eventSignatureIsEmpty(*eventPtr)) {
    logger::info("sumeragi") << "Signature exists and peer size is "
                             << eventPtr->peerSignatures()->size();
    // Check if we have at least 2f+1 signatures needed for Byzantine fault
    // tolerance ToDo re write
    if (
        eventPtr->peerSignatures() != nullptr &&
        eventPtr->peerSignatures()->size() >= context->maxFaulty * 2 + 1
    ) {
      logger::info("sumeragi") << "Signature exists and sig > 2*f + 1";

      logger::explore("sumeragi") << "0--------------------------0";
      logger::explore("sumeragi") << "+~~~~~~~~~~~~~~~~~~~~~~~~~~+";
      logger::explore("sumeragi") << "|Would you agree with this?|";
      logger::explore("sumeragi") << "+~~~~~~~~~~~~~~~~~~~~~~~~~~+";
      logger::explore("sumeragi") << "\033[93m0================================"
                                     "================================0\033[0m";
      logger::explore("sumeragi") <<  "\033[93m0\033[1m"  <<
        detail::hash(*eventPtr->transactions()->Get(0))  <<  "0\033[0m";
      logger::explore("sumeragi") << "\033[93m0================================"
                                     "================================0\033[0m";

      if(eventPtr->peerSignatures() != nullptr){
          detail::printJudge(
              // ToDo Re write
              eventPtr->peerSignatures()->size(),
              context->numValidatingPeers, context->maxFaulty * 2 + 1
          );
      }
      detail::printAgree();
      // Check Merkle roots to see if match for new state
      // TODO: std::vector<std::string>>const merkleSignatures =
      // event.merkleRootSignatures;
      // Try applying transaction locally and compute the merkle root

      // Commit locally
      logger::explore("sumeragi") << "commit";

      context->commitedCount++;

        logger::explore("sumeragi") << "commit count:" << context->commitedCount;
        logger::explore("sumeragi") << "prev commit :" << (eventPtr->code() == iroha::Code_COMMIT) ;

        auto commitedEventUniqPtr = flatbuffer_service::makeCommit(
            *eventPtr
        );

        eventPtr = flatbuffers::GetRoot<::iroha::ConsensusEvent>(commitedEventUniqPtr.get());
        logger::explore("sumeragi") << "done commit :" << (eventPtr->code() == iroha::Code_COMMIT) ;
        connection::iroha::SumeragiImpl::Verify::sendAll(
            *eventPtr
        );

    } else {
      logger::info("sumeragi") << "Signature exists and sig not enough";
       if(eventPtr->peerSignatures() != nullptr) {
           logger::info("sumeragi") <<"Length: "<< eventPtr->peerSignatures()->size();
       }

      /*
      // This is a new event, so we should verify, sign, and broadcast it
      auto newEventUniqPtr = flatbuffer_service::addSignature(
          *eventPtr,
          config::PeerServiceConfig::getInstance().getMyPublicKeyWithDefault(
              "Invalid"),  // ??
          "");
      auto newEventPtr =
          flatbuffers::GetRoot<::iroha::ConsensusEvent>(newEventUniqPtr.get());
      if (newEventPtr->peerSignatures() != nullptr) {
        logger::info("sumeragi")
            << "New Length: " << newEventPtr->peerSignatures()->size();
      }
      */
      const auto& event = *eventPtr;

      logger::info("sumeragi")
          << "tail public key is "
          << context->validatingPeers.at(context->proxyTailNdx)->publicKey;
      logger::info("sumeragi") << "tail is " << context->proxyTailNdx;
      logger::info("sumeragi")
          << "my public key is "
          << config::PeerServiceConfig::getInstance().getMyPublicKeyWithDefault(
                 "Invalid");

      if (context->validatingPeers.at(context->proxyTailNdx)->publicKey ==
          config::PeerServiceConfig::getInstance().getMyPublicKeyWithDefault(
              "Invalid")) {
        logger::info("sumeragi")
            << "I will send event to "
            << context->validatingPeers.at(context->proxyTailNdx)->ip;

        connection::iroha::SumeragiImpl::Verify::send(
            context->validatingPeers.at(context->proxyTailNdx)->ip,
            event);  // Think In Process
      } else {
        logger::info("sumeragi")
            << "Send All! sig:[" << event.peerSignatures()->size() << "]";

        connection::iroha::SumeragiImpl::Verify::sendAll(event);
        //
      }

      timer::setAwkTimerForCurrentThread(3000, [&]() {
          panic(event);
      });
    }
  }
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
* the set of considered validators, A, is expanded by f (e.g., by 1 in the
* example below):
*  ________________________    __________
* /           A            \  /    B     \
* |---|  |---|  |---|  |---|  |---|  |---|
* | 0 |--| 1 |--| 2 |--| 3 |--| 4 |--| 5 |
* |---|  |---|  |---|  |---|  |---|  |---|.
*/
void panic(const ConsensusEvent &event) {
  context->panicCount++;  // TODO: reset this later
  auto broadcastStart =
      2 * context->maxFaulty + 1 + context->maxFaulty * context->panicCount;
  auto broadcastEnd = broadcastStart + context->maxFaulty;

  // Do some bounds checking
  if (broadcastStart > context->numValidatingPeers - 1) {
    broadcastStart = context->numValidatingPeers - 1;
  }

  if (broadcastEnd > context->numValidatingPeers - 1) {
    broadcastEnd = context->numValidatingPeers - 1;
  }

  logger::info("sumeragi") << "broadcastEnd:" << broadcastEnd;
  logger::info("sumeragi") << "broadcastStart:" << broadcastStart;
  // WIP issue hash event
  // connection::sendAll(event->transaction().hash()); //TODO: change this to
  // only broadcast to peer range between broadcastStart and broadcastEnd
}

/**
 * The consensus order is based primarily on the trust scores. If two trust
 * scores are the same, then the order (ascending) of the public keys for the
 * servers are used.
 */
void determineConsensusOrder() {
  // WIP We creat getTrustScore() function. till then circle list
  /*
  std::deque<
          std::unique_ptr<peer::Node>
  > tmp_deq;
  for(int i=1;i<context->validatingPeers.size();i++){
      tmp_deq.push_back(std::move(context->validatingPeers[i]));
  }
  tmp_deq.push_back(std::move(context->validatingPeers[0]));
  context->validatingPeers.clear();
  context->validatingPeers = std::move(tmp_deq);

  std::sort(context->validatingPeers.begin(), context->validatingPeers.end(),
        [](const std::unique_ptr<peer::Node> &lhs,
           const std::unique_ptr<peer::Node> &rhs) {
            return lhs->getTrustScore() > rhs->getTrustScore()
                   || (lhs->getTrustScore() == rhs->getTrustScore()
                       && lhs->getPublicKey() < rhs->getPublicKey());
        }
  );
  logger::info("sumeragi")        <<  "determineConsensusOrder sorted!";
  logger::info("sumeragi")        <<  "determineConsensusOrder myPubkey:"     <<
  context->myPublicKey;

  for(const auto& peer : context->validatingPeers) {
      logger::info("sumeragi")    <<  "determineConsensusOrder PublicKey:"    <<
  peer->getPublicKey(); logger::info("sumeragi")    <<  "determineConsensusOrder
  ip:"           <<  peer->getIP();
  }
  */
  // context->isSumeragi = context->validatingPeers.at(0)->getPublicKey() ==
  // context->myPublicKey;
}

};  // namespace sumeragi