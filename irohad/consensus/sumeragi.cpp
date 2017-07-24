/*
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
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

#include <crypto/crypto.hpp>
#include <validation/stateful_validator.hpp>
#include <torii/command_service.hpp>
#include <logger/logger.hpp>
#include <peer_service/self_state.hpp>
#include <peer_service/monitor.hpp>
#include <timer/timer.hpp>
#include <datetime/time.hpp>
#include <vector>
#include <set>
#include <common/types.hpp>
#include <common/byteutils.hpp>
#include <block.pb.h>

#include <consensus/connection/service.hpp>
#include <consensus/connection/client.hpp>
#include <consensus/sumeragi.hpp>

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

namespace consensus {
  namespace sumeragi {

    using iroha::protocol::Block;
    using iroha::protocol::Signature;

    logger::Logger log("sumeragi");

    void initialize() {

    }

    size_t getMaxFaulty() {
      return (size_t) peer_service::monitor::getActivePeerSize() / 3;
    }

    size_t getNumValidatingPeers() {
      return getMaxFaulty() * 2 + 1;
    }

    bool unicast(const iroha::protocol::Block &block, size_t peerOrder) {
      return false;
    }

    bool leaderMulticast(const iroha::protocol::Block &block) {
      // connection::multicastWithRange(block, 1, getNumValidatingPeers());
      /*
      auto peerSize = getNumValidatingPeers();
      for (size_t i = 0; i < peerSize; i++) {
        unicast(block, i); // Currently, return value is not used.
      }
      */
      return true;
    }

    bool commit(const iroha::protocol::Block &block) {
      // connection::multicastAll(block);
      /*
      auto peerSize = (size_t)peer_service::monitor::getActivePeerSize();
      for (size_t i = 0; i < peerSize; i++) {
        unicast(block, i); // Currently, return value is not used.
      }
      */
      return true;
    }

    // TODO: Append block to db and calc merkle root.
    std::vector<uint8_t> appendBlock(const Block &block) {
      return std::vector<uint8_t>();
    }

    Block createSignedBlock(const Block &block,
                            const std::vector<uint8_t> &merkleRoot) {
      Block ret;
      return ret;
    }

    void setTimeOutCommit(const Block &block) {
      timer::setAwkTimerForCurrentThread(3000, [block] {
        panic(block);
      });
    }

    /**
     * returns expected tail to send committed block.
     * if returned value = -1, all peers has been used.
     */

    int getNextOrder() {
      return 0;
    }

    size_t countValidSignatures(const Block &block) {
      size_t numValidSignatures = 0;
      return numValidSignatures;
    }

    void processBlock(const Block &block) {


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
 * the set of considered validators, A, is expanded by 1.
 *  ________________________    __________
 * /           A            \  /    B     \
 * |---|  |---|  |---|  |---|  |---|  |---|
 * | 0 |--| 1 |--| 2 |--| 3 |--| 4 |--| 5 |
 * |---|  |---|  |---|  |---|  |---|  |---|.
 */

    void panic(const Block &block) {

    }

  }  // namespace sumeragi
}  // namespace consensus
