//
// Created by Dumitru Savva on 27/11/2017.
//

#ifndef IROHA_PROTO_GET_ACCOUNT_H
#define IROHA_PROTO_GET_ACCOUNT_H

#include "interfaces/queries/get_account.hpp"

#include "queries.pb.h"
#include "utils/lazy_initializer.hpp"
#include "utils/reference_holder.hpp"

namespace shared_model {
  namespace proto {
    class GetAccount final : public CopyableProto<interface::GetAccount,
                                                  iroha::protocol::Query,
                                                  GetAccount> {
     public:
      template <typename QueryType>
      explicit GetAccount(QueryType &&query)
          : CopyableProto(std::forward<QueryType>(query)),

            account_id_(detail::makeReferenceGenerator(
                &proto_->payload(),
                &iroha::protocol::Query::Payload::get_account)) {}

      GetAccount(const GetAccount &o) : GetAccount(o.proto_) {}

      GetAccount(GetAccount &&o) noexcept : GetAccount(std::move(o.proto_)) {}

      const interface::types::AccountIdType &accountId() const override {
        return account_id_->account_id();
      }

     private:
      // ------------------------------| fields |-------------------------------

      const detail::LazyInitializer<const iroha::protocol::GetAccount &>
          account_id_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_GET_ACCOUNT_H
