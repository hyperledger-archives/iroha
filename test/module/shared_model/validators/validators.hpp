/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_VALIDATOR_MOCKS_HPP
#define IROHA_VALIDATOR_MOCKS_HPP

#include "validators/abstract_validator.hpp"

#include <gmock/gmock.h>

namespace shared_model {
  namespace validation {

    // TODO: kamilsa 01.02.2018 IR-873 Replace all these validators with mock
    // classes

    struct AlwaysValidValidator {
      template <typename T>
      Answer validate(const T &) const {
        return {};
      }
    };

    template <typename T>
    class MockValidator : public AbstractValidator<T> {
     public:
      MOCK_CONST_METHOD1_T(validate, Answer(const T &));
    };

    struct AlwaysValidFieldValidator final {
      void validateAccountId(...) const {}
      void validateAssetId(...) const {}
      void validatePeer(...) const {}
      void validateAmount(...) const {}
      void validatePubkey(...) const {}
      void validatePeerAddress(...) const {}
      void validateRoleId(...) const {}
      void validateAccountName(...) const {}
      void validateDomainId(...) const {}
      void validateAssetName(...) const {}
      void validateAccountDetailKey(...) const {}
      void validateAccountDetailValue(...) const {}
      void validatePrecision(...) const {}
      void validateRolePermission(...) const {}
      void validateGrantablePermission(...) const {}
      void validateQuorum(...) const {}
      void validateCreatorAccountId(...) const {}
      void validateCreatedTime(...) const {}
      void validateCounter(...) const {}
      void validateSignatures(...) const {}
      void validateQueryPayloadMeta(...) const {}
      void validateDescription(...) const {}
      void validateBatchMeta(...) const {}
      void validateHeight(...) const {}
      void validateHash(...) const {}
    };

    template <typename Model>
    struct AlwaysValidModelValidator final : public AbstractValidator<Model> {
     public:
      Answer validate(const Model &m) const override{return {};};
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_VALIDATOR_MOCKS_HPP
