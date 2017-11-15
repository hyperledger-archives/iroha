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

TEST(DomainNameParserTest, HandleValidDomainName) {
  ASSERT_TRUE(DomainNameValidator::isValid("a"));
  ASSERT_TRUE(DomainNameValidator::isValid("ab"));
  ASSERT_TRUE(DomainNameValidator::isValid("abc"));
  ASSERT_TRUE(DomainNameValidator::isValid("abc.efg"));
  ASSERT_TRUE(DomainNameValidator::isValid("abc.efg.hij"));
  ASSERT_TRUE(DomainNameValidator::isValid("u9EEA432F"));
  ASSERT_TRUE(DomainNameValidator::isValid("a-hyphen"));
  ASSERT_TRUE(DomainNameValidator::isValid("altplus.com"));
  ASSERT_TRUE(DomainNameValidator::isValid("altplus.com.jp"));
  ASSERT_TRUE(DomainNameValidator::isValid(
      "maxLabelLengthIs63paddingPaddingPaddingPaddingPaddingPaddingPad"));
  ASSERT_TRUE(DomainNameValidator::isValid("endWith0"));
}

TEST(DomainNameValidatorTest, HandleInvalidDomainName) {
  ASSERT_FALSE(DomainNameValidator::isValid(" "));
  ASSERT_FALSE(DomainNameValidator::isValid("9start.with.non.letter"));
  ASSERT_FALSE(DomainNameValidator::isValid("-startWithDash"));
  ASSERT_FALSE(DomainNameValidator::isValid("@.is.not.allowed"));
  ASSERT_FALSE(DomainNameValidator::isValid("no space is allowed"));
  ASSERT_FALSE(DomainNameValidator::isValid("endWith-"));
  ASSERT_FALSE(DomainNameValidator::isValid("label.endedWith-.is.not.allowed"));
  ASSERT_FALSE(DomainNameValidator::isValid(
      "aLabelMustNotExceeds63charactersALabelMustNotExceeds63characters"));
}
