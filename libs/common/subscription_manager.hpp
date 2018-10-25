/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_SUBSCRIPTION_MANAGER_HPP
#define IROHA_SUBSCRIPTION_MANAGER_HPP

#include <rxcpp/rx.hpp>

#include <algorithm>
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
      virtual ~SubscriptionManager() {
        std::for_each(
            subscriptions.begin(),
            subscriptions.end(),
            [this](const auto &subscription) { subscription.unsubscribe(); });
      }

     protected:
      /**
       * Add new subscription for the management
       */
      void addSubscription(rxcpp::composite_subscription &&subscription) {
        subscriptions.push_back(subscription);
      }

      /// list of all subscriptions
      std::vector<rxcpp::composite_subscription> subscriptions;
    };
  }  // namespace utils
}  // namespace iroha
#endif  // IROHA_SUBSCRIPTION_MANAGER_HPP
