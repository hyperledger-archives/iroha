//
// Created by Dumitru Savva on 27/11/2017.
//

#ifndef IROHA_PROTO_GET_ACCOUNT_H
#define IROHA_PROTO_GET_ACCOUNT_H

#include "interfaces/queries/get_account.hpp"

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
                account_id_(
                        [this] {
                          return query_->payload().get_account().account_id();
                        })
                 {}

      GetAccount(const GetAccount &o)
              : GetAccount(*o.query_) {}

      GetAccount(GetAccount &&o) noexcept
              : GetAccount(std::move(o.query_.variant())) {}

      const interface::types::AccountIdType &accountId() const override {
        return *account_id_;
      }

      ModelType *copy() const override {
        auto tmp = iroha::protocol::Query(*query_);
        return new GetAccount(tmp);
      }

    private:
      // ------------------------------| fields |-------------------------------

      // proto
      detail::ReferenceHolder<iroha::protocol::Query> query_;


      const detail::LazyInitializer<const std::string&> account_id_;

    };

  }  // namespace proto
}  // namespace shared_model

#endif //IROHA_PROTO_GET_ACCOUNT_H
