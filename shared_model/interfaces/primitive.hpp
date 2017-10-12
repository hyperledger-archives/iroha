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

#ifndef IROHA_PRIMITIVE_HPP
#define IROHA_PRIMITIVE_HPP

#include <typeinfo>
#include <string>

namespace shared_model {
  namespace interface {

    class Primitive {
      virtual std::string toString()  const {
        using namespace std::string_literals;
        // TODO rework with format
        return "Primitive at address["s  + "]";
      }

      // may be make as friend
      bool operator==(const Primitive &primitive) const {
        return typeid(*this) == typeid(primitive) ? equals(primitive) : false;
      }

      bool operator!=(const Primitive &primitive)  const {
        return not (*this == primitive);
      }

      virtual Primitive *copy()  const = 0;

     private:
      // may be provide as template
      virtual bool equals(const Primitive &primitive) const = 0;
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_PRIMITIVE_HPP
