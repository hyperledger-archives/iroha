/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __IROHA_PEER_SERVICE_PEER_SERVIEC_HPP__
#define __IROHA_PEER_SERVICE_PEER_SERVIEC_HPP__

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include <commands.pb.h>
#include <model/block.hpp>
#include <model/peer.hpp>
#include <peer_service/self_status.hpp>

namespace peer_service {

  using Peer = iroha::model::Peer;
  using Command = iroha::protocol::Command;

  void initialize();

  /**
   * @return List of peers that therefore permutation.
   */
  std::vector<std::shared_ptr<Peer>> getPermutationPeers();

  /**
   * @param i index
   * @return A i-th peer that therefore permutation.
   */
  Peer getPermutationAt(int i);

  /**
   * @return List of peers that is used by ordering service.
   */
  std::vector<std::shared_ptr<Peer>> getOrderingPeers();

  /**
   * @return List of peers that is used by ordering service and is that will
   * be send sumeragi.
   */
  std::vector<std::shared_ptr<Peer>> getActiveOrderingPeers();

  /**
   * @return self status
   */
  const SelfStatus& self();

  /**
   * When on_porposal sends on_commit, it is called.
   * It check signs from Block, and identify dead peers which It throw to
   * issue Peer::Remove transaction.
   * @param commited_block commited block with signs
   */
  void RemoveDeadPeers(const iroha::model::Block& commited_block);

  /**
   * When on_commit, it is called.
   * It change peer oreder.
   */
  void changePermutation();

  /**
   * When commit fails, it is called.
   * It throw to issue Peer::Stop(Self) transaction.
   */
  void selfStop();

  /**
   * When commit successes and state of self peer is UnSynced, It is called.
   * It throw to issue Peer::Activate(self) transaction.
   */
  void selfActivate();

  /**
   * validate command
   */
  void validate(const Command::Peer::Add&);
  void validate(const Command::Peer::Remove&);
  void validate(const Command::Peer::Activate&);
  void validate(const Command::Peer::Stop&);
  void validate(const Command::Peer::ChangeRole&);

  /**
   * execute command
   */
  void execute(const Command::Peer::Add&);
  void execute(const Command::Peer::Remove&);
  void execute(const Command::Peer::Activate&);
  void execute(const Command::Peer::Stop&);
  void execute(const Command::Peer::ChangeRole&);

  namespace detail {
    void issueStop(const std::string& ip, const Peer& stop_peer);
    void issueActivate(const std::string& ip, const Peer& activate_peer);

    SelfStatus self_;
    std::vector<int> permutation_;
    std::vector<int> active_ordering_permutation_;

    std::vector<std::shared_ptr<Peer>> peers_;
  }

}  // namespace peer_service

#endif
