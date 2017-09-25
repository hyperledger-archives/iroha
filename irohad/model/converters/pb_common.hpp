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

#ifndef IROHA_PB_COMMON_HPP
#define IROHA_PB_COMMON_HPP

#include "commands.pb.h"
#include "amount/amount.hpp"

namespace iroha {
  namespace model {
    namespace converters {

      // amount
      protocol::Amount serializeAmount(iroha::Amount iroha_amount);
      iroha::Amount deserializeAmount(protocol::Amount pb_amount);
    }
  }
}

#endif  // IROHA_PB_COMMON_HPP
