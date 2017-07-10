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

#pragma once

#include <model/block.hpp>
#include <model/proposal.hpp>
#include <model/transaction.hpp>
#include <model/transaction_response.hpp>
#include <model/asset.hpp>
#include <model/account.hpp>
#include <model/wallet.hpp>
#include <model/peer.hpp>
#include <model/client.hpp>
#include <model/peer.hpp>
#include <model/model_crypto_provider.hpp>
#include <model/model_hash_provider.hpp>

// commands
#include <model/command.hpp>
#include <model/commands/add_peer.hpp>
#include <model/commands/add_signatory.hpp>
#include <model/commands/create_account.hpp>
#include <model/commands/create_asset.hpp>
#include <model/commands/create_wallet.hpp>
#include <model/commands/issue_asset.hpp>
#include <model/commands/set_permissions.hpp>
#include <model/commands/set_quorum.hpp>
#include <model/commands/transfer_asset.hpp>

// query
#include <model/query.hpp>
#include <model/queries/get_account.hpp>
#include <model/queries/get_signatories.hpp>
#include <model/queries/get_transactions.hpp>
#include <model/queries/get_wallets.hpp>

// query response
#include <model/query_response.hpp>
#include <model/queries/responses/account_response.hpp>
#include <model/queries/responses/error_response.hpp>
#include <model/queries/responses/signatories_response.hpp>
#include <model/queries/responses/transactions_response.hpp>
#include <model/queries/responses/wallets_response.hpp>


/**
 * Model module provides objects that are useful for all other modules in system.
 * Model objects do not depend on transport, such as protobuf.
 * Model objects in general are structures with public immutable fileds.
*/
