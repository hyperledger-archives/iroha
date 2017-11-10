#ifndef IROHA_DNS_PARSER_HPP
#define IROHA_DNS_PARSER_HPP

#include <string>

namespace iroha {
  namespace model {
    /**
     * Parse a string as a domain name as defined in RFC 1035.
     */
    class DnsParser {
     public:
      // clang-format off
      /**
       * Check if the given string `str` is in valid domain syntax defined in
       * the RFC 1035. Return the result of the validation.
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
       * @return true, if the validation is successful.
       */
      // clang-format on
      static bool isValid(const std::string &str);
    };
  }  // namespace model
}  // namespace iroha

#endif /* IROHA_DNS_PARSER_HPP */
