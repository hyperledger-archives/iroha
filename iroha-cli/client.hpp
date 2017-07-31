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

#ifndef IROHA_CLIENT_HPP
#define IROHA_CLIENT_HPP

#include <fstream>
#include <model/transaction.hpp>
#include <string>
#include "ametsuchi/block_serializer.hpp"
#include "common/types.hpp"
#include "torii/command_client.hpp"

namespace iroha_cli {

  class CliClient {
   public:
    explicit CliClient(std::string target_ip, int port,
                       std::string account_name);
    /**
     * Send transaction to Iroha-Network
     * @param json_tx
     * @return
     */
    std::string sendTx(std::string json_tx);

   private:
    torii::CommandSyncClient client_;
    std::string client_priv_key_;
    std::string client_pub_key_;
  };
}  // namespace iroha_cli

#endif  // IROHA_CLIENT_CPP_HPP
