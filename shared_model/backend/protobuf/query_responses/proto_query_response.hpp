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

#ifndef IROHA_PROTO_QUERY_RESPONSE_HPP
#define IROHA_PROTO_QUERY_RESPONSE_HPP

#include "backend/protobuf/query_responses/proto_account_asset_response.hpp"
#include "interfaces/queries/query.hpp"
#include "interfaces/query_responses/query_response.hpp"
#include "responses.pb.h"
#include "utils/lazy_initializer.hpp"
#include "utils/variant_deserializer.hpp"

template <typename... T, typename Archive>
auto loadQueryResponse(Archive &&ar) {
  int which =
      ar.GetDescriptor()->FindFieldByNumber(ar.response_case())->index();
  return shared_model::detail::variant_impl<T...>::template load<
      shared_model::interface::QueryResponse::QueryResponseVariantType>(
      std::forward<Archive>(ar), which);
}

namespace shared_model {
  namespace proto {
    class QueryResponse final : public interface::QueryResponse {
     private:
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      using LazyVariantType = Lazy<QueryResponseVariantType>;

     public:
      /// type of proto variant
      using ProtoQueryResponseVariantType =
          boost::variant<w<AccountAssetResponse>>;

      /// list of types in variant
      using ProtoQueryResponseListType = ProtoQueryResponseVariantType::types;

      /// Type of query hash
      using QueryHashType = interface::Query::HashType;

      template <typename QueryResponseType>
      explicit QueryResponse(QueryResponseType &&queryResponse)
          : queryResponse_(std::forward<QueryResponseType>(queryResponse)),
            variant_([this] {
              return loadQueryResponse<ProtoQueryResponseListType>(
                  *queryResponse_);
            }),
            hash_([this] {
              return QueryHashType(queryResponse_->query_hash());
            }) {}

      QueryResponse(const QueryResponse &o)
          : QueryResponse(*o.queryResponse_) {}

      QueryResponse(QueryResponse &&o) noexcept
          : QueryResponse(std::move(o.queryResponse_.variant())) {}

      const QueryResponseVariantType &get() const override { return *variant_; }

      const QueryHashType &queryHash() const override { return *hash_; }

      ModelType *copy() const override {
        return new QueryResponse(
            iroha::protocol::QueryResponse(*queryResponse_));
      }

     private:
      detail::ReferenceHolder<iroha::protocol::QueryResponse> queryResponse_;

      const LazyVariantType variant_;

      const Lazy<QueryHashType> hash_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_QUERY_RESPONSE_HPP
