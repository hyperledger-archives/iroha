/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_BROADCAST_HPP
#define IROHA_BROADCAST_HPP

class AgreementPhase<template Emitted, template Received, template Outcome> {
  AgreementPhase(SenderNetwork network) {
    network.subscribe([this](Peer, Recieved) {
      // multi-threaded - no guarantees
      onReceive()
    });
  }

  /// one initiate generates one outcome - only for yac
  void initiate(Emitted value, PeerList peers, Round r) {
    terminate(coordinator_->initiate(value, peers, r));
  }

  observable<Outcome> outcome();

 private:
  void terminate(Optional<Outcome> terminate) {
    if (terminate) {
      outcome_events_.onNext(*terminate);
    }
  }

  void onReceive(Peer peer, Received received_value) {
    if (is_active_phase) { // is it required ?
      terminate(coordinator_->onReceiveOnTime(peer, received_value));
    } else {
      terminate(coordinator_->onReceiveOutOfTime(peer, received_value));
    }
  }

  /**
   * Main logic of the phase
   */
  Coordinator<Outcome> coordinator_;

  ReceiverNetwork<Peer, Received> receiver_;

  subject<Outcome> outcome_events_;

  Emitted own_value_;
  bool is_active_phase;  // takeWhile instead?
  mutex mutex_;          // ?? mb in rx
};

class Broadcast<typename Value> {
  /// observable is managed by outer scope
  void broadcast(Value, observable<Peer> on_next_) {
    on_next_subscriber =
        on_next_.subscribe([this](auto peer) { network_->send(peer, value); });
  }

  EmitterNetwork<Peer, Emitted> network_;
};

class YacRoundCoordinator<template Value, template Outcome> {
  Supermajority checker_;
  VoteStorage vote_storage_;
  subject<Peer, Outcome> out_bus;

  // -------------------------------| One round |-------------------------------

  Round current_round;
  Value value_;
};

// usage

int main() {
  using ConsensusOutcomeFirstPhase = variant<Commit, Undecided, Reject>;
  using ConsensusOutcome = variant<Commit, Reject>;
  AgreementPhase<Vote, vector<Vote>, FirstPhaseOutcome> yac_first_phase;

  AgreementPhase<Candidate, vector<Candidate>, ConsensOutcome> yac_second_phase;





  // in consensus gate

  yac_first_phase.initiate(...).outcome().subscribe([](FirstPhaseOutcome outcome) {
    if (isUndecided(outcome)) {
      return yac_second_phase.initiate(...).outcome().first();
    } else {
      return outcome;
    }
  });
}

#endif  // IROHA_BROADCAST_HPP
