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

#ifndef IROHA_SHARED_MODEL_AMOUNT_HPP
#define IROHA_SHARED_MODEL_AMOUNT_HPP

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
#include "amount/amount.hpp"
#include "interfaces/common_objects/amount.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/polymorphic_wrapper.hpp"
#include "interfaces/primitive.hpp"
#include "utils/string_builder.hpp"

namespace shared_model {
  namespace interface {
    class Amount : public Primitive<Amount, iroha::Amount> {
     public:
      /**
       * Converts to uint64_t vector
       * @return amount represented as vector of uint64_t
       */
      virtual std::vector<uint64_t> toUint64s() const = 0;

      /**
       * Gets integer representation value, which ignores precision
       * @return amount represented as integer value, which ignores precision
       */
      virtual boost::multiprecision::uint256_t intValue() const = 0;

      /**
       * Gets the position of precision
       * @return the position of precision
       */
      virtual uint8_t precision() const = 0;

      /**
       * Takes percentage from current amount
       * @param percents
       * @return percentage representation of amount value
       */
      virtual detail::PolymorphicWrapper<Amount> percentage(
          boost::multiprecision::uint256_t percents) const = 0;

      /**
       * Takes percentage represented as amount value
       * The current scale and scale of percents may differ
       * @param percents
       * @return percentage representation of amount value
       */
      virtual detail::PolymorphicWrapper<Amount> percentage(
          const Amount &percents) const = 0;

      /**
       * Comparisons are possible between amounts with different precisions.
       */

      /**
       * Checks equality of objects inside
       * @param rhs - other wrapped value
       * @return true, if wrapped objects are same
       */
      bool operator==(const ModelType &rhs) const override {
        return intValue() == rhs.intValue() and precision() == rhs.precision();
      }

      virtual bool operator<(const Amount &) const = 0;
      virtual bool operator>(const Amount &) const = 0;
      virtual bool operator<=(const Amount &) const = 0;
      virtual bool operator>=(const Amount &) const = 0;

      /**
       * Stringify the data.
       * @return the content of asset.
       */
      std::string toString() const override {
        return detail::PrettyStringBuilder()
          .init("Amount")
          .append("intValue", intValue().str())
          .append("precision", std::to_string(precision()))
          .finalize();
      }

      /**
       * Makes old model.
       * @return An allocated old model of account asset response.
       */
      OldModelType *makeOldModel() const override {
        return new OldModelType(intValue(), precision());
      }
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_AMOUNT_HPP
