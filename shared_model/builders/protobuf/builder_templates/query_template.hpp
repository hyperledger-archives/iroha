/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_PROTO_QUERY_BUILDER_TEMPLATE_HPP
#define IROHA_PROTO_QUERY_BUILDER_TEMPLATE_HPP

#include <boost/range/adaptor/transformed.hpp>

#include "backend/protobuf/queries/proto_query.hpp"
#include "builders/protobuf/unsigned_proto.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/transaction.hpp"
#include "queries.pb.h"
#include "validators/default_validator.hpp"

namespace shared_model {
  namespace proto {

    using QueryFieldsMaskType = int;

    enum class QueryFields {
      CreatedTime,
      CreatorAccountId,
      QueryType,
      QueryCounter,
      LAST_DEFAULT_REQUIRED = QueryCounter,
      TxPageMeta,
      TOTAL
    };
    static const auto kQueryFieldsTotal =
        static_cast<std::underlying_type<QueryFields>::type>(
            QueryFields::TOTAL);

    static_assert(kQueryFieldsTotal <= sizeof(QueryFieldsMaskType) * 8,
                  "QueryFieldsMaskType is not large enough to hold any "
                  "QueryFields values.");

    static constexpr QueryFieldsMaskType fieldMask(QueryFields f) {
      return 1 << static_cast<std::underlying_type<QueryFields>::type>(f);
    }

    const QueryFieldsMaskType kDefaultRequiredQueryFields =
        (fieldMask(QueryFields::LAST_DEFAULT_REQUIRED) << 1) - 1;

    /**
     * Template query builder for creating new types of query builders by
     * means of replacing template parameters
     * @tparam CheckFields -- check that all required fields are set
     * @tparam SV -- stateless validator called when build method is invoked
     * @tparam BT -- build type of built object returned by build method
     * @tparam RequiredFieldsMask -- field counter for checking that all
     * required fields are set
     */
    template <bool CheckFields = true,
              typename SV = validation::DefaultUnsignedQueryValidator,
              typename BT = UnsignedWrapper<Query>,
              QueryFieldsMaskType RequiredFieldsMask =
                  kDefaultRequiredQueryFields>
    class DEPRECATED TemplateQueryBuilder {
      template <bool, typename, typename, QueryFieldsMaskType>
      friend class TemplateQueryBuilder;

      using ProtoQuery = iroha::protocol::Query;

     // for some reason the friend declaration above does not work =(
     public:
      template <int Sp>
      TemplateQueryBuilder(
          const TemplateQueryBuilder<CheckFields, SV, BT, Sp> &o)
          : query_(o.query_), stateless_validator_(o.stateless_validator_) {}
     private:

      template <QueryFieldsMaskType set, QueryFieldsMaskType requested>
      using BuilderWithAlteredFields =
          TemplateQueryBuilder<CheckFields,
                               SV,
                               BT,
                               (RequiredFieldsMask & ~set) | requested>;

      /**
       * Make transformation on copied content
       * @tparam set_fields - the field to mark as set in the result.
       * @tparam requested_fields - the field to mark as required in the result.
       * @return new builder with updated state
       */
      template <QueryFieldsMaskType set_fields,
                QueryFieldsMaskType requested_fields = 0>
      auto updateFields() {
        static_assert(RequiredFieldsMask & set_fields,
                      "Unsuitable field tried to be set.");
        return *reinterpret_cast<
            BuilderWithAlteredFields<set_fields, requested_fields> *>(this);
      }

     public:
      /// The type of builder whith all fields set.
      using Completed = TemplateQueryBuilder<CheckFields, SV, BT, 0>;

      static constexpr bool is_completed = RequiredFieldsMask == 0;

      TemplateQueryBuilder(const SV &validator = SV())
          : stateless_validator_(validator) {}

      auto createdTime(interface::types::TimestampType created_time) {
        query_.mutable_payload()->mutable_meta()->set_created_time(
            created_time);
        return updateFields<fieldMask(QueryFields::CreatedTime)>();
      }

      auto creatorAccountId(
          const interface::types::AccountIdType &creator_account_id) {
        query_.mutable_payload()->mutable_meta()->set_creator_account_id(
            creator_account_id);
        return updateFields<fieldMask(QueryFields::CreatorAccountId)>();
      }

      auto queryCounter(interface::types::CounterType query_counter) {
        query_.mutable_payload()->mutable_meta()->set_query_counter(
            query_counter);
        return updateFields<fieldMask(QueryFields::QueryCounter)>();
      }

      auto getAccount(const interface::types::AccountIdType &account_id) {
        query_.mutable_payload()->mutable_get_account()->set_account_id(
            account_id);
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getSignatories(const interface::types::AccountIdType &account_id) {
        query_.mutable_payload()->mutable_get_signatories()->set_account_id(
            account_id);
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getAccountTransactions(
          const interface::types::AccountIdType &account_id) {
        query_.mutable_payload()
            ->mutable_get_account_transactions()
            ->set_account_id(account_id);
        return updateFields<fieldMask(QueryFields::QueryType)>();
        // return updateFields<fieldMask(QueryFields::QueryType),
        // fieldMask(QueryFields::TxPageMeta)>();
      }

      auto getAccountAssetTransactions(
          const interface::types::AccountIdType &account_id,
          const interface::types::AssetIdType &asset_id) {
        auto query =
            query_.mutable_payload()->mutable_get_account_asset_transactions();
        query->set_account_id(account_id);
        query->set_asset_id(asset_id);
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getAccountAssets(const interface::types::AccountIdType &account_id) {
        query_.mutable_payload()->mutable_get_account_assets()->set_account_id(
            account_id);
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getAccountDetail(
          const interface::types::AccountIdType &account_id = "",
          const interface::types::AccountDetailKeyType &key = "",
          const interface::types::AccountIdType &writer = "") {
        auto query = query_.mutable_payload()->mutable_get_account_detail();
        if (not account_id.empty()) {
          query->set_account_id(account_id);
        }
        if (not key.empty()) {
          query->set_key(key);
        }
        if (not writer.empty()) {
          query->set_writer(writer);
        }
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getRoles() {
        query_.mutable_payload()->mutable_get_roles();
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getAssetInfo(const interface::types::AssetIdType &asset_id) {
        query_.mutable_payload()->mutable_get_asset_info()->set_asset_id(
            asset_id);
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getRolePermissions(const interface::types::RoleIdType &role_id) {
        query_.mutable_payload()->mutable_get_role_permissions()->set_role_id(
            role_id);
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      template <typename Collection>
      auto getTransactions(const Collection &hashes) {
        for (const auto &hash :
             hashes | boost::adaptors::transformed(crypto::toBinaryString)) {
          query_.mutable_payload()->mutable_get_transactions()->add_tx_hashes(
              hash);
        }
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto getTransactions(
          std::initializer_list<interface::types::HashType> hashes) {
        return getTransactions(hashes);
      }

      template <typename... Hash>
      auto getTransactions(const Hash &... hashes) {
        return getTransactions({hashes...});
      }

      auto getPendingTransactions() {
        query_.mutable_payload()->mutable_get_pending_transactions();
        return updateFields<fieldMask(QueryFields::QueryType)>();
      }

      auto build() const {
        static_assert(!CheckFields || RequiredFieldsMask == 0,
                      "Required fields are not set");
        if (not query_.has_payload()) {
          throw std::invalid_argument("Query missing payload");
        }
        if (query_.payload().query_case()
            == iroha::protocol::Query_Payload::QueryCase::QUERY_NOT_SET) {
          throw std::invalid_argument("Missing concrete query");
        }
        auto result = Query(iroha::protocol::Query(query_));
        auto answer = stateless_validator_.validate(result);
        if (answer.hasErrors()) {
          throw std::invalid_argument(answer.reason());
        }
        return BT(std::move(result));
      }

      //static const int total = kQueryFieldsTotal;

     private:
      ProtoQuery query_;
      SV stateless_validator_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_QUERY_BUILDER_TEMPLATE_HPP
