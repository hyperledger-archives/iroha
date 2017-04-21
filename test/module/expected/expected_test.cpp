/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
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

#include <gtest/gtest.h>
#include <utils/expected.hpp>

TEST(UseExpected, expectedWithException) {
    auto whatsNumber = [](int number) -> Expected<std::string> {
        if (number == 123) {
            return std::string("YES");
        }
        if (number == 321) {
            return makeUnexpected(exception::crypto::InvalidKeyException("Hoge"));
        }
        return makeUnexpected(exception::IrohaException("Invalid Number"));
    };

    auto res = whatsNumber(123);
    if (res) {
        ASSERT_STREQ((*res).c_str(), "YES");
        ASSERT_STREQ(res.value().c_str(), "YES");
    } else if (res = whatsNumber(321)) {
        std::string str = *res;
        ASSERT_STREQ(str.c_str(), "Keyfile is invalid, cause is: Hoge");
    } else {
        ASSERT_STREQ(res.message(), "Invalid Number");
    }
}
