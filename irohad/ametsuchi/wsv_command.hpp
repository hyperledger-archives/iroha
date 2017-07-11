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

#ifndef IROHA_WSV_COMMAND_HPP
#define IROHA_WSV_COMMAND_HPP

namespace iroha {
  namespace ametsuchi {

    /**
     * Commands for modifying world state view
     */
    class WsvCommand {
     public:
      virtual ~WsvCommand() = 0;

      /**
       * Update or insert account
       * @param account
       * @return true if no error occurred, false otherwise
       */
      virtual bool upsertAccount(const model::Account &account) = 0;

      /**
       * Insert asset
       * @param asset
       * @return
       */
      virtual bool insertAsset(const model::Asset &asset) = 0;

      /**
       * Update or insert wallet
       * @param wallet
       * @return
       */
      virtual bool upsertWallet(const model::Wallet &wallet) = 0;

      /**
       * Insert signatory
       * @param master_key
       * @param signatory
       * @return
       */
      virtual bool insertSignatory(const ed25519::pubkey_t master_key,
                                   const ed25519::pubkey_t signatory) = 0;

      /**
       * Delete signatory
       * @param master_key
       * @param signatory
       * @return
       */
      virtual bool deleteSignatory(const ed25519::pubkey_t master_key,
                                   const ed25519::pubkey_t signatory) = 0;

      /**
       * Update or insert peer
       * @param peer
       * @return
       */
      virtual bool upsertPeer(const model::Peer &peer) = 0;

      /**
       * Delete peer
       * @param peer
       * @return
       */
      virtual bool deletePeer(const model::Peer &peer) = 0;
    };

  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_WSV_COMMAND_HPP
