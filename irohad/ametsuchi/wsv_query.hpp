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

#ifndef IROHA_WSV_QUERY_HPP
#define IROHA_WSV_QUERY_HPP

#include <common/types.hpp>
#include <model/model.hpp>
#include <string>
#include <vector>

namespace iroha {
  namespace ametsuchi {

    /**
     *  Public interface for world state view queries
     */
    class WsvQuery {
     public:
      virtual ~WsvQuery() = default;

      /**
       * Get account by user master key
       * @param master_key
       * @return
       */
      virtual model::Account getAccount(
          const ed25519::pubkey_t &master_key) = 0;

      /**
       * Get signatories of account by user master key
       * @param master_key
       * @return
       */
      virtual std::vector<ed25519::pubkey_t> getSignatories(
          const ed25519::pubkey_t &master_key) = 0;

      /**
       * Get asset by its name
       * @param asset_id
       * @return
       */
      virtual model::Asset getAsset(const std::string &asset_id) = 0;

      /**
       * Get wallet of user
       * @param master_key
       * @param asset_id
       * @return
       */
      virtual model::Wallet getWallet(const ed25519::pubkey_t &master_key,
                                      const std::string &asset_id) = 0;

      /**
       * Get peer by given IP address
       * @param address
       * @return
       */
      virtual model::Peer getPeer(const std::string &address) = 0;
    };

  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_WSV_QUERY_HPP
