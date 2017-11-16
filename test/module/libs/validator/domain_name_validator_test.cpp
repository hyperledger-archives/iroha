/**
 * Copyright AltPlus Inc., Ltd. 2017 All Rights Reserved.
 * http://en.altplus.co.jp
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

#include "validator/domain_name_validator.hpp"
#include <gtest/gtest.h>

using namespace validator;

TEST(DomainNameValidatorTest, HandleValidDomainName) {
  ASSERT_TRUE(isValidDomainName("a"));
  ASSERT_TRUE(isValidDomainName("ab"));
  ASSERT_TRUE(isValidDomainName("abc"));
  ASSERT_TRUE(isValidDomainName("abc.efg"));
  ASSERT_TRUE(isValidDomainName("abc.efg.hij"));
  ASSERT_TRUE(isValidDomainName("u9EEA432F"));
  ASSERT_TRUE(isValidDomainName("a-hyphen"));
  ASSERT_TRUE(isValidDomainName("altplus.com"));
  ASSERT_TRUE(isValidDomainName("altplus.com.jp"));
  ASSERT_TRUE(isValidDomainName(
      "maxLabelLengthIs63paddingPaddingPaddingPaddingPaddingPaddingPad"));
  ASSERT_TRUE(isValidDomainName("endWith0"));
}

TEST(DomainNameValidatorTest, HandleInvalidDomainName) {
  ASSERT_FALSE(isValidDomainName(" "));
  ASSERT_FALSE(isValidDomainName("9start.with.non.letter"));
  ASSERT_FALSE(isValidDomainName("-startWithDash"));
  ASSERT_FALSE(isValidDomainName("@.is.not.allowed"));
  ASSERT_FALSE(isValidDomainName("no space is allowed"));
  ASSERT_FALSE(isValidDomainName("endWith-"));
  ASSERT_FALSE(isValidDomainName("label.endedWith-.is.not.allowed"));
  ASSERT_FALSE(isValidDomainName(
      "aLabelMustNotExceeds63charactersALabelMustNotExceeds63characters"));
}
