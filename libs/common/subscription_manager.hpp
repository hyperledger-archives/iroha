/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_SUBSCRIPTION_MANAGER_HPP
#define IROHA_SUBSCRIPTION_MANAGER_HPP

#include <rxcpp/rx.hpp>
#include <vector>

namespace iroha {
  namespace utils {
    /**
     * Class is responsible for managing subscriptions
     * Typical use case of the class is private inheritance and pushing own
     * subscriptions via addSubscription method. Dtor guarantee that all
     * subscriptions will be unsubscribed.
     * Note: class is NOT thread-safe
     */
    class SubscriptionManager {
     public:
      virtual ~SubscriptionManager();

     protected:
      /**
       * Add new subscription for the management
       */
      void addSubscription(rxcpp::composite_subscription &&subscription);

      /// list of all subscriptions
      std::vector<rxcpp::composite_subscription> subscriptions;
    };
  }  // namespace utils
}  // namespace iroha
#endif  // IROHA_SUBSCRIPTION_MANAGER_HPP
