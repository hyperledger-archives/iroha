#include "model/execution/dns_parser.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_optional.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_plus.hpp>

#include <iostream>

namespace iroha {
  namespace model {
    bool DnsParser::isValid(const std::string &str) {
      namespace qi = boost::spirit::qi;

      qi::rule<decltype(str.begin())> letter, let_dig, end_with_let_dig,
          let_dig_hyp, label, subdomain, domain;
      domain = subdomain | ' ';
      subdomain = label % '.';
      // I could not express the grammer [ [ <ldh-str> ] <let-dig> ] with
      // <ldh-str> directly. So I introduced a intermediate rule
      // end_with_let_dig instead of <ldh-str>.
      label = letter >> -end_with_let_dig;
      end_with_let_dig =
          ((let_dig_hyp | let_dig) >> (end_with_let_dig | let_dig)) | let_dig;
      let_dig_hyp = '-' | let_dig;
      let_dig = letter | qi::digit;
      letter = qi::lower | qi::upper;

      auto f = str.begin();
      auto const result = qi::parse(f, str.end(), domain);
      return result && (f == str.end());
    }
  }  // namespace model
}  // namespace iroha
