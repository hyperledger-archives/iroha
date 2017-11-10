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

#ifndef IROHA_LAZY_INITIALIZER_HPP
#define IROHA_LAZY_INITIALIZER_HPP

#include <functional>
#include <memory>

namespace shared_model {
  namespace detail {

    /**
     * Lazy class for lazy converting one type to another
     * @tparam Source - input type
     * @tparam Target - output type
     */
    template <typename Target>
    class LazyInitializer {
     private:
      /// Type of transformation
      using TransformType = std::function<Target()>;

     public:
      LazyInitializer(const TransformType &transform) : transform_(transform) {}

      LazyInitializer(const LazyInitializer &) = default;

      /**
       * @return value after transformation
       */
      const Target &get() const {
        if (target_value_ == nullptr) {
          target_value_ = std::make_shared<Target>(transform_());
        }
        return *target_value_;
      }

     private:
      TransformType transform_;
      mutable std::shared_ptr<Target> target_value_;
    };

    /**
     * Function for creating lazy object
     * @tparam Source - source type
     * @tparam Transform - type of transformation
     * @param source - source value
     * @param transform - transformation instance
     * @return initialized lazy value
     */
    template <typename Transform>
    auto makeLazyInitializer(Transform &&transform) {
      using targetType = decltype(transform());
      return LazyInitializer<targetType>(std::forward<Transform>(transform));
    }
  }  // namespace detail
}  // namespace shared_model
#endif  // IROHA_LAZY_INITIALIZER_HPP
