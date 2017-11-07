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

#ifndef IROHA_LAZY_HPP
#define IROHA_LAZY_HPP

#include <functional>
#include <memory>

namespace shared_model {
  namespace detail {

    /**
     * Lazy class for lazy converting one type to another
     * @tparam Source - input type
     * @tparam Target - output type
     */
    template <typename Source, typename Target>
    class Lazy {
     private:
      /// Type of transformation
      using TransformType = std::function<Target(const Source &)>;

     public:
      Lazy(const Source &source, const TransformType &transform)
          : source_(source), transform_(transform) {}

      /**
       * @return value after transformation
       */
      const Target &get() {
        if (target_value_ == nullptr) {
          target_value_ = std::make_shared<Target>(transform_(source_));
        }
        return *target_value_;
      }

     private:
      Source source_;
      TransformType transform_;
      std::shared_ptr<Target> target_value_;
    };

    /**
     * Function for creating lazy object
     * @tparam Source - source type
     * @tparam Transform - type of transformation
     * @param source - source value
     * @param transform - transformation instance
     * @return initialized lazy value
     */
    template <typename Source, typename Transform>
    auto makeLazy(Source &&source, Transform &&transform) {
      using targetType = decltype(transform(source));
      return Lazy<Source, targetType>(source, transform);
    }
  }  // namespace detail
}  // namespace shared_model
#endif  // IROHA_LAZY_HPP
