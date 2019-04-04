/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_POINTER_UTILS_HPP
#define IROHA_POINTER_UTILS_HPP

#include <functional>

template <typename L, typename Less, typename R = L>
class DereferenceLess {
 public:
  bool operator()(const L &lhs, const R &rhs) const {
    return Less()(lhs, rhs);
  }
};

template <typename Ptr, typename Hash>
class DereferenceHash {
 public:
  size_t operator()(const Ptr &lhs) const {
    return Hash()(*lhs);
  }
};

#endif  // IROHA_POINTER_UTILS_HPP
