/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "backend/protobuf/queries/proto_tx_pagination_meta.hpp"

#include "cryptography/hash.hpp"

namespace types = shared_model::interface::types;

using namespace shared_model::proto;

TxPaginationMeta::TxPaginationMeta(const TransportType &query)
    : CopyableProto(query) {}

/*
TxPaginationMeta::TxPaginationMeta(TransportType &&query)
    : CopyableProto(std::move<TransportType>(query)) {}
*/

TxPaginationMeta::TxPaginationMeta(const TxPaginationMeta &o)
    : TxPaginationMeta(*o.proto_) {}

TxPaginationMeta::TxPaginationMeta(TxPaginationMeta &&o) noexcept
    : CopyableProto(std::move(*o.proto_)) {}

types::TransactionsNumberType TxPaginationMeta::pageSize() const {
  return proto_->page_size();
}

boost::optional<types::HashType> TxPaginationMeta::firstTxHash() const {
  return {proto_->opt_first_tx_hash_case(),
          types::HashType(proto_->first_tx_hash())};
}
