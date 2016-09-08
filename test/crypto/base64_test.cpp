
#include "../../core/crypto/base64.hpp"
#include "../../core/util/testutil.hpp"

#include <gtest/gtest.h>

TEST(Base64, EncodeAndDecode){
    unsigned char* text = (unsigned char*)"Hey, I'm mizuki";
    ASSERT_EQ( text, base64::decode(base64::encode(text)));
}

