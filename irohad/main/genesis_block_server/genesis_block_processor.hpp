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

#ifndef IROHA_GENESIS_BLOCK_PROCESSOR_IMPL_HPP
#define IROHA_GENESIS_BLOCK_PROCESSOR_IMPL_HPP

#include <model/block.hpp>
#include <endpoint.grpc.pb.h>
#include "ametsuchi/temporary_factory.hpp"
#include "ametsuchi/mutable_factory.hpp"
#include "genesis_block_processor.hpp"

namespace iroha {

  class GenesisBlockProcessor {
  public:
    explicit GenesisBlockProcessor(ametsuchi::MutableFactory &mutable_factory)
      : mutable_factory_(mutable_factory) {}

    ~GenesisBlockProcessor() {}
    bool genesis_block_handle(const iroha::model::Block &block);

  private:
    ametsuchi::MutableFactory &mutable_factory_;
  };
}

#endif  // IROHA_GENESIS_BLOCK_PROCESSOR_IMPL_HPP
