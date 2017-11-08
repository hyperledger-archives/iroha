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

#ifndef IROHA_VARIANT_DESERIALIZER_HPP
#define IROHA_VARIANT_DESERIALIZER_HPP

#include <boost/serialization/variant.hpp>

namespace shared_model {
  namespace detail {
    template <class S>
    struct variant_impl {
      struct load_null {
        template <class Archive, class V>
        static void invoke(Archive &, int, V &) {}
      };

      struct load_impl {
        template <class Archive, class V>
        static void invoke(Archive &ar, int which, V &v) {
          if (which == 0) {
            using head_type = typename boost::mpl::front<S>::type;
            v = head_type(ar);
          } else {
            using type = typename boost::mpl::pop_front<S>::type;
            variant_impl<type>::load(ar, which - 1, v);
          }
        }
      };

      template <class Archive, class V>
      static void load(Archive &ar, int which, V &v) {
        using typex =
            typename boost::mpl::eval_if<boost::mpl::empty<S>,
                                         boost::mpl::identity<load_null>,
                                         boost::mpl::identity<load_impl>>::type;
        typex::invoke(ar, which, v);
      }
    };
  }  // namespace detail
}  // namespace shared_model

#endif  // IROHA_VARIANT_DESERIALIZER_HPP
