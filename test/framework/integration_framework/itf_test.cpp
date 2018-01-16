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

#include "framework/integration_framework/integration_test_framework.hpp"

namespace integration_framework {

  /**
   * @given unsigned empty GetAccount query
   * AND default-initialized IntegrationTestFramework
   * @when query is sent to the framework
   * @then query response is STATELESS_INVALID
   */
  TEST(ItfTest, SendQueryWithValidation) {
    iroha::model::GetAccount query;
    IntegrationTestFramework()
        .setInitialState()
        .sendQuery(
            query,
            [](const auto &res) {
              try {
                auto err_res =
                    dynamic_cast<const iroha::model::ErrorResponse &>(res);
                ASSERT_EQ(iroha::model::ErrorResponse::STATELESS_INVALID,
                          err_res.reason);
              } catch (const std::bad_cast &e) {
                FAIL() << "Unexpected response received";
              }
            })
        .done();
  }
}  // namespace integration_framework
