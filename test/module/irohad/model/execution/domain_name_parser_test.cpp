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

#include "model/execution/domain_name_parser.hpp"
#include <gtest/gtest.h>

using namespace iroha::model;

TEST(DomainNameParserTest, HandleValidDomainName) {
  ASSERT_TRUE(DomainNameParser::isValid("a"));
  ASSERT_TRUE(DomainNameParser::isValid("ab"));
  ASSERT_TRUE(DomainNameParser::isValid("abc"));
  ASSERT_TRUE(DomainNameParser::isValid("abc.efg"));
  ASSERT_TRUE(DomainNameParser::isValid("abc.efg.hij"));
  ASSERT_TRUE(DomainNameParser::isValid("u9EEA432F"));
  ASSERT_TRUE(DomainNameParser::isValid("a-hyphen"));
  ASSERT_TRUE(DomainNameParser::isValid("altplus.com"));
  ASSERT_TRUE(DomainNameParser::isValid("altplus.com.jp"));
  ASSERT_TRUE(DomainNameParser::isValid(
      "maxLabelLengthIs63paddingPaddingPaddingPaddingPaddingPaddingPad"));
  ASSERT_TRUE(DomainNameParser::isValid("endWith0"));
}

TEST(DomainNameParserTest, HandleInvalidDomainName) {
  ASSERT_FALSE(DomainNameParser::isValid(" "));
  ASSERT_FALSE(DomainNameParser::isValid("9start.with.non.letter"));
  ASSERT_FALSE(DomainNameParser::isValid("-startWithDash"));
  ASSERT_FALSE(DomainNameParser::isValid("@.is.not.allowed"));
  ASSERT_FALSE(DomainNameParser::isValid("no space is allowed"));
  ASSERT_FALSE(DomainNameParser::isValid("endWith-"));
  ASSERT_FALSE(DomainNameParser::isValid("label.endedWith-.is.not.allowed"));
  ASSERT_FALSE(DomainNameParser::isValid(
      "aLabelMustNotExceeds63charactersALabelMustNotExceeds63characters"));
}
