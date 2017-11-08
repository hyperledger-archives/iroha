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

#ifndef IROHA_POLYMORPHIC_WRAPPER_HPP
#define IROHA_POLYMORPHIC_WRAPPER_HPP

#include <memory>

namespace shared_model {
  namespace detail {

    /**
     * Wrapper for polymorphic types for correct working with constructors
     * @tparam T - type of wrapped object
     */
    template <class T>
    class PolymorphicWrapper final {
      template <typename>
      friend class PolymorphicWrapper;

     public:
      /// Type of wrapped object
      using WrappedType = T;

      /**
       * Empty constructor
       * TODO: this constructor required for building boost::variant object, but
       * semantics may be broken
       */
      PolymorphicWrapper() = default;

      /**
       * Value constructor
       * @param value - pointer for wrapping
       */
      template <typename Y>
      PolymorphicWrapper(const Y *value) : ptr(std::shared_ptr<Y>(value)) {}

      template <typename... Args>
      PolymorphicWrapper(Args &&... args)
          : ptr(std::make_shared<T>(std::forward<Args>(args)...)) {}

      template <typename Y>
      PolymorphicWrapper(const PolymorphicWrapper<Y> &rhs)
          : ptr(std::shared_ptr<T>(rhs.ptr->copy())) {}

      template <typename Y>
      PolymorphicWrapper(PolymorphicWrapper<Y> &&rhs) noexcept : ptr(rhs.ptr) {
        rhs.ptr = nullptr;
      }
      /**
       * Copy constructor that performs deep copy
       * @param rhs - another wrapped value
       */
      PolymorphicWrapper(const PolymorphicWrapper &rhs)
          : ptr(std::shared_ptr<T>(rhs.ptr->copy())) {}

      /**
       * Move constructor
       * @param rhs - wrapped temporary value
       */
      PolymorphicWrapper(PolymorphicWrapper &&rhs) noexcept : ptr(nullptr) {
        std::swap(this->ptr, rhs.ptr);
      }

      /**
       * Copy operator=
       * @param rhs - another wrapped value
       * @return *this
       */
      PolymorphicWrapper &operator=(const PolymorphicWrapper &rhs) {
        ptr = std::shared_ptr<T>(rhs.ptr->copy());
        return *this;
      }

      /**
       * Move operator=
       * @param rhs - another temporary wrapped value
       * @return *this
       */
      PolymorphicWrapper &operator=(PolymorphicWrapper &&rhs) noexcept {
        std::swap(this->ptr, rhs.ptr);
        return *this;
      }

      /**
       * Checks equality of objects inside
       * @param rhs - other wrapped value
       * @return true, if wrapped objects are same
       */
      bool operator==(const PolymorphicWrapper &rhs) const {
        return *ptr == *rhs.ptr;
      }

      /**
       * Mutable wrapped object pointer
       * @return pointer for wrapped object
       */
      WrappedType *operator->() { return ptr.get(); }

      /**
       * Immutable wrapped object pointer
       * @return pointer for wrapped object
       */
      const WrappedType *operator->() const { return ptr.get(); }

     private:
      /// pointer with wrapped value
      std::shared_ptr<WrappedType> ptr;
    };

  }  // namespace detail
}  // namespace shared_model

#endif  // IROHA_POLYMORPHIC_WRAPPER_HPP
