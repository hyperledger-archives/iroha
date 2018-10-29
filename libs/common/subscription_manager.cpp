/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common/subscription_manager.hpp"

#include <algorithm>

using namespace iroha::utils;

SubscriptionManager::~SubscriptionManager() {
  std::for_each(
      subscriptions.begin(),
      subscriptions.end(),
      [this](const auto &subscription) { subscription.unsubscribe(); });
}

void SubscriptionManager::addSubscription(
    rxcpp::composite_subscription &&subscription) {
  subscriptions.push_back(subscription);
}
