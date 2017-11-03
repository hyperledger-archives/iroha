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

#ifndef IROHA_SHARED_MODEL_ERROR_RESPONSE_HPP
#define IROHA_SHARED_MODEL_ERROR_RESPONSE_HPP

#include <boost/variant.hpp>
#include "crypto/hash.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/primitive.hpp"
#include "interfaces/visitor_apply_for_all.hpp"
#include "model/queries/responses/error_response.hpp"
#include "utils/string_builder.hpp"

namespace shared_model {
  namespace interface {
    /**
     * Provide error answer with reason about error
     */
    class ErrorResponse
        : public Primitive<ErrorResponse, iroha::model::ErrorResponse> {
     public:
      /**
       * Reason of error
       */
      struct StatelessInvalid final {
        std::string toString() const { return "STATELESS_INVALID"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::STATELESS_INVALID;
        }
        constexpr bool operator==(StatelessInvalid const&) const { return true; }
      };
      struct StatefulInvalid final {
        std::string toString() const { return "STATEFUL_INVALID"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::STATEFUL_INVALID;
        }
        constexpr bool operator==(StatefulInvalid const&) const { return true; }
      };
      struct NoAccount final {
        std::string toString() const { return "NO_ACCOUNT"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::NO_ACCOUNT;
        }
        constexpr bool operator==(NoAccount const&) const { return true; }
      };
      struct NoAsset final {
        std::string toString() const { return "NO_ASSET"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::NO_ASSET;
        }
        constexpr bool operator==(NoAsset const&) const { return true; }
      };
      struct NoRoles final {
        std::string toString() const { return "NO_ROLES"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::NO_ROLES;
        }
        constexpr bool operator==(NoRoles const&) const { return true; }
      };
      struct NoAccountAssets final {
        using T = NoAccountAssets;
        std::string toString() const { return "NO_ACCOUNT_ASSETS"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::NO_ACCOUNT_ASSETS;
        }
        constexpr bool operator==(NoAccountAssets const&) const { return true; }
      };
      struct NoSignatories final {
        std::string toString() const { return "NO_SIGNATORIES"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::NO_SIGNATORIES;
        }
        constexpr bool operator==(NoSignatories const&) const { return true; }
      };
      struct NotSupported final {
        std::string toString() const { return "NOT_SUPPORTED"; }
        [[deprecated]] OldModelType::Reason makeOldModel() const {
          return OldModelType::Reason::NOT_SUPPORTED;
        }
        constexpr bool operator==(NotSupported const&) const { return true; }
      };

      using Reason = boost::variant<StatelessInvalid,
                                    StatefulInvalid,
                                    NoAccount,
                                    NoAsset,
                                    NoRoles,
                                    NoAccountAssets,
                                    NoSignatories,
                                    NotSupported>;
      /**
       * Error reason for failing in fetching data.
       * @return Error reason
       */
      virtual Reason &reason() const = 0;

      /**
       * Stringify the data.
       * @return the content of error response.
       */
      std::string toString() const override {
        return detail::PrettyStringBuilder()
            .init("ErrorResponse")
            .append("reason",
                    boost::apply_visitor(detail::ToStringVisitor(), reason()))
            .finalize();
      }

      /**
       * Implementation of operator ==
       * @param rhs - the right hand-side of ErrorResponse object
       * @return true if they have same values.
       */
      bool operator==(const ModelType &rhs) const override {
        return reason() == rhs.reason();
      }

      /**
       * Makes old model.
       * @return An allocated old model of error response.
       */
      OldModelType *makeOldModel() const override {
        OldModelType *oldModel = new OldModelType();
        oldModel->reason = boost::apply_visitor(
            detail::OldModelCreatorVisitor<OldModelType::Reason>(), reason());
        return oldModel;
      }
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_ERROR_RESPONSE_HPP
