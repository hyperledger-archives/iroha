/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_SIMULATOR_HPP
#define IROHA_SIMULATOR_HPP

#include "simulator/block_creator.hpp"
#include "simulator/verified_proposal_creator.hpp"
#include "validation/stateful_validator.hpp"

namespace iroha {
  namespace simulator {

    class Simulator : public VerifiedProposalCreator {
     public:
      Simulator(validation::StatefulValidator& statefulValidator, ametsuchi::TemporaryFactory& factory);

      void process_proposal(model::Proposal proposal) override;

      rxcpp::observable<model::Proposal> on_verified_proposal() override;



     private:
      // internal
      rxcpp::subjects::subject<model::Proposal> notifier_;
      validation::StatefulValidator& validator_;
      ametsuchi::TemporaryFactory& ametsuchi_factory_;
    };
  }  // namespace simulator
}  // namespace iroha

#endif  // IROHA_SIMULATOR_HPP
