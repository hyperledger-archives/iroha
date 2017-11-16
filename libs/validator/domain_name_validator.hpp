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

#ifndef VALIDATOR_DOMAIN_NAME_VALIDATOR_HPP
#define VALIDATOR_DOMAIN_NAME_VALIDATOR_HPP

#include <string>

namespace validator {
  // clang-format off
  /**
   * Check if the given string `str` is in valid domain syntax defined in
   * the RFC 1035 and 1123. Return the result of the validation.
   *
   * The domain syntax in RFC 1035 is given below:
   *
   *   <domain>      ::= <subdomain> | ” ”
   *   <subdomain>   ::= <label> | <subdomain> “.” <label>
   *   <label>       ::= <letter> [ [ <ldh-str> ] <let-dig> ]
   *   <ldh-str>     ::= <let-dig-hyp> | <let-dig-hyp> <ldh-str>
   *   <let-dig-hyp> ::= <let-dig> | “-”
   *   <let-dig>     ::= <letter> | <digit>
   *   <letter>      ::= any one of the 52 alphabetic characters A through Z in
   *                     upper case and a through z in lower case
   *   <digit>       ::= any one of the ten digits 0 through 9
   *
   * And the subsequent RFC 1123 disallows the root white space.
   *
   * @return true, if the validation is successful.
   */
  // clang-format on
  bool isValidDomainName(const std::string &str);

}  // namespace validator

#endif /* VALIDATOR_DOMAIN_NAME_VALIDATOR_HPP */
