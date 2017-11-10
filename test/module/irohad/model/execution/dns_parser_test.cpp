#include <gtest/gtest.h>
#include "model/execution/dns_parser.hpp"

using namespace iroha::model;

TEST(DnsParserTest, HandleValidDns){
    ASSERT_TRUE(DnsParser::isValid("a"));
    ASSERT_TRUE(DnsParser::isValid("ab"));
    ASSERT_TRUE(DnsParser::isValid("abc"));
    ASSERT_TRUE(DnsParser::isValid("abc.efg"));
    ASSERT_TRUE(DnsParser::isValid("abc.efg.hij"));
    ASSERT_TRUE(DnsParser::isValid("u9EEA432F"));
    ASSERT_TRUE(DnsParser::isValid("a-hyphen"));
    ASSERT_TRUE(DnsParser::isValid("altplus.com"));
    ASSERT_TRUE(DnsParser::isValid("altplus.com.jp"));
}

TEST(DnsParserTest, HandleInvalidDns){
    ASSERT_FALSE(DnsParser::isValid("9start.with.non.letter"));
    ASSERT_FALSE(DnsParser::isValid("@.is.not.allowed"));
    ASSERT_FALSE(DnsParser::isValid("no space is allowed"));
    ASSERT_FALSE(DnsParser::isValid("endWith-"));
}


