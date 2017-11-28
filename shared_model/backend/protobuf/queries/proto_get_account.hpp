//
// Created by Dumitru Savva on 27/11/2017.
//

#ifndef IROHA_PROTO_GET_ACCOUNT_H
#define IROHA_PROTO_GET_ACCOUNT_H

#include "interfaces/queries/get_account.hpp.hpp"

#include "backend/protobuf/common_objects/amount.hpp"
#include "queries.pb.h"
#include "utils/lazy_initializer.hpp"
#include "utils/reference_holder.hpp"

namespace shared_model {
  namespace proto {
    class GetAccount final : public interface::GetAccount {
    public:
      template <typename QueryType>
      explicit GetAccount(QueryType &&query)
              : query_(std::forward<QueryType>(query)),
                get_account_(
                        [this] { return query->account_id(); })
                 {}

      GetAccount(const GetAccount &o)
              : GetAccount(*o.query_) {}

      GetAccount(GetAccount &&o) noexcept
              : GetAccount(std::move(o.query_.variant())) {}

      types::AccountIdType &accountId() const override {
        return get_account_->account_id();
      }

      ModelType *copy() const override {
        return new GetAccount(iroha::protocol::Query(*query_));
      }

    private:
      // ------------------------------| fields |-------------------------------

      // proto
      detail::ReferenceHolder<iroha::protocol::Query> query_;

      // lazy
      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      const Lazy<const iroha::protocol::GetAccount &> get_account_;

    };

  }  // namespace proto
}  // namespace shared_model

#endif //IROHA_PROTO_GET_ACCOUNT_H
