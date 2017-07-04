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

#include <dao/block.hpp>
#include <dao/proposal.hpp>
#include <dao/transaction.hpp>
#include <dao/transaction_response.hpp>
#include <dao/asset.hpp>
#include <dao/account.hpp>
#include <dao/wallet.hpp>
#include <dao/peer.hpp>
#include <dao/domain.hpp>
#include <dao/client.hpp>
#include <dao/peer.hpp>
#include <dao/dao_crypto_provider.hpp>
#include <dao/dao_hash_provider.hpp>

// commands
#include <dao/command.hpp>
#include <dao/commands/add_peer.hpp>
#include <dao/commands/add_signature.hpp>
#include <dao/commands/create_account.hpp>
#include <dao/commands/create_asset.hpp>
#include <dao/commands/create_wallet.hpp>
#include <dao/commands/issue_asset.hpp>
#include <dao/commands/set_permissions.hpp>
#include <dao/commands/set_quorum.hpp>
#include <dao/commands/transfer_asset.hpp>

// query
#include <dao/query.hpp>
#include <dao/queries/get_account.hpp>
#include <dao/queries/get_blocks.hpp>
#include <dao/queries/get_identities.hpp>
#include <dao/queries/get_transactions.hpp>
#include <dao/queries/get_wallets.hpp>

// query response
#include <dao/query_response.hpp>
#include <dao/queries/responses/account_response.hpp>
#include <dao/queries/responses/blocks_response.hpp>
#include <dao/queries/responses/error_response.hpp>
#include <dao/queries/responses/identities_response.hpp>
#include <dao/queries/responses/transactions_response.hpp>
#include <dao/queries/responses/wallets_response.hpp>


/**
 * DAO - Data Access Object.
 * DAO module provides objects that are useful for all other modules in system.
 * DAO objects do not depend on transport, such as protobuf.
 * DAO objects in general are structures with public immutable fileds.
*/