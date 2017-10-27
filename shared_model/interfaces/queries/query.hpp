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

#ifndef IROHA_SHARED_MODEL_QUERY_HPP
#define IROHA_SHARED_MODEL_QUERY_HPP

#include <boost/variant.hpp>
#include "interfaces/common_objects/types.hpp"
#include "interfaces/polymorphic_wrapper.hpp"
#include "interfaces/primitive.hpp"
#include "interfaces/queries/get_account.hpp"
#include "interfaces/signable.hpp"
#include "interfaces/visitor_apply_for_all.hpp"
#include "model/query.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Class Query provides container with one of concrete query available in
     * system. General note: this class is container for queries but not a base
     * class.
     */
    class Query : public Signable<Query, iroha::model::Query> {
     private:
      /// Shortcut type for polymorphic wrapper
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

     public:
      /// Type of variant, that handle concrete query
      using QueryVariantType = boost::variant<w<GetAccount>>;

      /// Types of concrete commands, in attached variant
      using QueryListType = QueryVariantType::types;

      /**
       * @return reference to const variant with concrete command
       */
      virtual const QueryVariantType &get() const = 0;

      /**
       * @return id of query creator
       */
      virtual const types::AccountIdType &creatorAccountId() const = 0;

      /// Type of query counter
      using QueryCounterType = uint64_t;
      /**
       * Query counter - incremental variable reflect for number of sent to
       * system queries plus 1. Required for preventing replay attacks.
       * @return attached query counter
       */
      virtual const QueryCounterType &queryCounter() = 0;

      // ------------------------| Primitive override |-------------------------

      std::string toString() const override {
        return boost::apply_visitor(detail::ToStringVisitor(), get());
      }

      OldModelType *makeOldModel() const {
        return boost::apply_visitor(
            detail::OldModelCreatorVisitor<OldModelType *>(), get());
      }

      bool operator==(const ModelType &rhs) const override {
        return this->get() == rhs.get();
      }
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_QUERY_HPP
