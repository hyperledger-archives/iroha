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

#ifndef IROHA_SHARED_MODEL_PROTO_QUERY_HPP
#define IROHA_SHARED_MODEL_PROTO_QUERY_HPP

#include "backend/protobuf/queries/proto_get_account.hpp"
#include "queries.pb.h"
#include "interfaces/queries/query.hpp"
#include "utils/lazy_initializer.hpp"
#include "utils/variant_deserializer.hpp"

template <typename... T, typename Archive>
auto load_query(Archive &&ar) {
  int which = ar.GetDescriptor()->FindFieldByNumber(ar.command_case())->index();
  return shared_model::detail::variant_impl<T...>::
  template load<shared_model::interface::Query::QueryVariantType>(
          std::forward<Archive>(ar), which);
}

namespace shared_model {
  namespace proto {
    class Query final : public interface::Query {
    private:
      /// polymorphic wrapper type shortcut
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

      /// lazy variant shortcut
      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      using LazyVariantType = Lazy<QueryVariantType>;

    public:
      /// type of proto variant
      using ProtoQueryVariantType = boost::variant<w<GetAccount>>;

      /// list of types in proto variant
      using ProtoQueryListType = ProtoQueryVariantType::types;

      template <typename CommandType>
      explicit Query(QueryType &&query)
              : query_(std::forward<QueryType>(query)),
                variant_([this] { return load<ProtoQueryListType>(*query_); }),
                blob_([this] { return BlobType(query_->SerializeAsString()); }) {}

      Query(const Query &o) : Query(*o.query_) {}

      Query(Query &&o) noexcept : Query(std::move(o.query_.variant())) {
      }

      const QueryVariantType &get() const override { return *variant_; }

      const BlobType &blob() const override { return *blob_; }

      ModelType *copy() const override {
        return new Query(iroha::protocol::Query(*query_));
      }

    private:
      // ------------------------------| fields |-------------------------------

      // proto
      detail::ReferenceHolder<iroha::protocol::Query> query_;

      // lazy
      const LazyVariantType variant_;

      const Lazy<BlobType> blob_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_PROTO_COMMAND_HPP
