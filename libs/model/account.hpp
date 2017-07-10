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
#ifndef IROHA_ACCOUNT_HPP
#define IROHA_ACCOUNT_HPP

#include <model/model.hpp>
#include <string>

namespace iroha {
  namespace model {

    /**
     * Account Model
     */
    struct Account {

      struct Permissions {

        /**
         * Can account add assets to own account;
         * Dangerous operation - require high number of quorum;
         */
        bool issue_assets;

        /**
         * Can account create new type of assets;
         * Dangerous operation - require high number of quorum;
         */
        bool create_assets;

        /**
         * Can account read private information of other accounts;
         * Auditor's permission
         */
        bool read_all_accounts;

        /**
         * Available change only read_all_accounts and unlimited_transfer
         * Administrator's account permission
         */
        bool set_permissions;

        /**
         * Can account provide unlimited transfers to other accounts;
         * User's account permission
         */
        bool unlimited_transfer;
      };

      /**
       * Is this account banned for operations
       */
      bool is_banned;

      /**
       * Account permissions
       */
      Permissions permissions;

      /**
       * Minimum quorum of signatures need for transactions
       */
      uint32_t quorum;

      /**
       * Identifier of account
       */
      ed25519::pubkey_t pubkey;
    };
  }
}

#endif  // IROHA_ACCOUNT_HPP
