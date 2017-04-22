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


TEST(UseExpected, returnsValidData) {
  auto whatsNumber = [](int number) -> Expected<std::string> {
    if (number == 123) {
      return std::string("YES");
    }
    if (number == 321) {
      return makeUnexpected(exception::crypto::InvalidKeyException("Hoge"));
    }
    return makeUnexpected(
        exception::crypto::InvalidMessageLengthException("InvalidLength"));
  };

  auto res = whatsNumber(123);
  if (res) {
    ASSERT_TRUE(res);
    ASSERT_TRUE(res.valid());
    ASSERT_STREQ((*res).c_str(), "YES");
    ASSERT_STREQ(res.value().c_str(), "YES");
  } else {
    FAIL();
  }
}

TEST(UseExpected, returnsException1) {
  auto whatsNumber = [](int number) -> Expected<std::string> {
    if (number == 123) {
      return std::string("YES");
    }
    if (number == 321) {
      return makeUnexpected(exception::crypto::InvalidKeyException("Hoge"));
    }
    return makeUnexpected(
        exception::crypto::InvalidMessageLengthException("Foo"));
  };
  auto res = whatsNumber(321);
  if (res) {
    FAIL() << "Invalid valid()";
  } else {
    try {
      std::rethrow_exception(res.excptr());
    } catch (const exception::crypto::InvalidKeyException& e) {
      ASSERT_STREQ(res.error(), "Keyfile is invalid, cause is: Hoge");
    } catch (const exception::crypto::InvalidMessageLengthException& e) {
      FAIL() << "exception::crypto::InvalidMessageLengthException";
    } catch (const exception::IrohaException& e) {
      FAIL() << "Unknown exception: " << typeid(decltype(e)).name();
    }
  }
}

TEST(UseExpected, returnsException2) {
  auto whatsNumber = [](int number) -> Expected<std::string> {
    if (number == 123) {
      return std::string("YES");
    }
    if (number == 321) {
      return makeUnexpected(exception::crypto::InvalidKeyException("Hoge"));
    }
    return makeUnexpected(
        exception::crypto::InvalidMessageLengthException("Foo"));
  };
  auto res = whatsNumber(999);
  if (res) {
    FAIL() << "Invalid valid()";
  } else {
    try {
      std::rethrow_exception(res.excptr());
    } catch (const exception::crypto::InvalidKeyException& e) {
      FAIL() << "exception::crypto::InvalidKeyException";
    } catch (const exception::crypto::InvalidMessageLengthException& e) {
      ASSERT_STREQ(res.error(), "Message Foo has wrong length");
    } catch (const exception::IrohaException& e) {
      FAIL() << "Unknown exception: " << typeid(decltype(e)).name();
    }
  }
}

TEST(UserExpected, VoidHandler) {
  auto voidTest = [](int number) -> VoidHandler {
    if (number != 123) {
      return makeUnexpected(exception::IrohaException("Invalid"));
    }
    return {};
  };
  auto res = voidTest(123);
}
