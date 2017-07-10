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

#ifndef IROHA_TRANSFER_ASSET_HPP
#define IROHA_TRANSFER_ASSET_HPP

#include <model/model.hpp>
#include <string>

namespace iroha {
  namespace model {

    /**
     * Transfer asset from one account to another
     */
    struct TransferAsset : public Command {

      /**
       * Source wallet
       */
      std::string src_wallet_uuid;

      /**
       * Destination wallet
       */
      std::string dst_wallet_uuid;

      /**
       * Amount of transferred asset
       */
      std::string amount;
    };
  } // namespace model
} // namespace iroha
#endif //IROHA_TRANSFER_ASSET_HPP
