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

#ifndef IROHA_VISITOR_APPLY_FOR_ALL_HPP
#define IROHA_VISITOR_APPLY_FOR_ALL_HPP

#include <boost/variant/static_visitor.hpp>
#include <type_traits>

namespace shared_model {
  namespace detail {

/**
 * Macro for defining concept check of the member function
 */
#define DEF_HAS_FUNC(FUNC_NAME)                                          \
  template <typename ClassType>                                          \
  struct has_##FUNC_NAME {                                               \
   private:                                                              \
    template <typename T>                                                \
    static auto check(T x) -> decltype(x.FUNC_NAME(), std::true_type{}); \
    static std::false_type check(...);                                   \
                                                                         \
   public:                                                               \
    static bool const value =                                            \
        decltype(check(std::declval<ClassType>()))::value;               \
  };

    /**
     * Concept for checking to have toString()
     * @tparam ClassType - target class
     */
    DEF_HAS_FUNC(toString)

    /**
     * Concept for checking to have makeOldModel()
     * @tparam ClassType - target class
     */
    DEF_HAS_FUNC(makeOldModel)

    /**
     * Class provides generic toString visitor for objects
     */
    class ToStringVisitor : public boost::static_visitor<std::string> {
     public:
      template <typename InputType,
                std::enable_if_t<not has_toString<InputType>::value,
                                 std::nullptr_t> = nullptr>
      std::string operator()(InputType &operand) const {
        return operand->toString();
      }
      template <typename InputType,
                std::enable_if_t<has_toString<InputType>::value,
                                 std::nullptr_t> = nullptr>
      std::string operator()(InputType &operand) const {
        return operand.toString();
      }
    };

    /**
     * Class provides generic converter for old-fashion domain objects
     * @tparam T abstract return type
     */
    template <typename T>
    class OldModelCreatorVisitor : public boost::static_visitor<T> {
     public:
      template <typename InputType,
                std::enable_if_t<not has_makeOldModel<InputType>::value,
                                 std::nullptr_t> = nullptr>
      T operator()(InputType &operand) const {
        return operand->makeOldModel();
      }

      template <typename InputType,
                std::enable_if_t<has_makeOldModel<InputType>::value,
                                 std::nullptr_t> = nullptr>
      T operator()(InputType &operand) const {
        return operand.makeOldModel();
      }
    };

  }  // namespace detail
}  // namespace shared_model
#endif  // IROHA_VISITOR_APPLY_FOR_ALL_HPP
