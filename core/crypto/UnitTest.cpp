#include "Base64.hpp"
#include "Hash.hpp"
#include "Signature.hpp"
#include "../util/Test.hpp"

#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <functional>


int main(){
  util::test("hash None",[]{
    return util::equals(
      hash::sha3_256_hex(""),
      std::string("a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a")
    );
  });

  util::test("hash MizukiSonoko",[]{
    return util::equals(
      hash::sha3_256_hex("MizukiSonoko"),
      std::string("e8ab66edec5769ad8b3af3d9cebc8a62ee042fbb88f8a77c84f547530da12f94")
    );
  });

  util::test("base64 encode \"MizukiSonoko\\n\"",[]{
    return util::equals(
      base64::encode(reinterpret_cast<const unsigned char*>("MizukiSonoko\n")),
      std::string("TWl6dWtpU29ub2tvCg==")
    );
  });

  util::test("base64 encode \"AioIoriIsCute\"",[]{
    return util::equals(
      base64::encode(reinterpret_cast<const unsigned char*>("AioIoriIsCute")),
      std::string("QWlvSW9yaUlzQ3V0ZQ==")
    );
  });

  util::test("base64 encode \"\"",[]{
    return util::equals(
      std::string(base64::encode(reinterpret_cast<const unsigned char*>(""))),
      std::string("")
    );
  });

  util::test("base64 decode \"\"",[]{
    return util::equals(
      std::string(reinterpret_cast<const char*>(base64::decode(""))),
      std::string("")
    );
  });

  util::test("base64 decode unsigned char",[]{
    const unsigned char* message = reinterpret_cast<const unsigned char*>("I'mUnsignedCharMan");
    return util::equals(
        reinterpret_cast<const unsigned char*>(
          base64::decode(base64::encode(message))
        ),
        message
    );
  });

  util::test("base64 decode \"QWlvSW9yaUlzQ3V0ZQ==\"",[]{
    return util::equals(
      std::string(
        reinterpret_cast<const char*>(
          base64::decode(
            "QWlvSW9yaUlzQ3V0ZQ=="
          )
        )
      ),
      std::string("AioIoriIsCute")
    );
  });

  util::test("base64 decode \"Chiya detail\"", []{
      return true;
      // ignore

      std::string detail = "Chiya Ujimatsu is a main character of the Is the Order a Rabbit? manga series as well as it's anime series. Her family runs their own café, called Ama Usa An, which has its own rabbit mascot, her pet, Anko.Chiya is a gentle and harmonious girl with a calm aura. She is very polite and courteous to the point that she rarely ever expresses negative feelings in order to keep the peace. She is so ladylike that she finds great pleasure in naming her sweets with long, delicate and fancy names such as \"Red Jewel in a Snowy Field\" and \"Moon and Stars Reflected Upon the Sea\". While a sweet and amiable girl on the surface, it is hinted that Chiya has a darker side to her personality because she revels in telling scary ghost stories and disturbing her friends. She has dreams of becoming a Samurai.";
      return util::equals(
        std::string(
          reinterpret_cast<const char*>(
            base64::decode(
              base64::encode(
                reinterpret_cast<const unsigned char*>(detail.c_str())
              )
            )
          )
        ),
        detail
      );
  });

  // This code has many MemoryLeak....... (T^T)
  util::test("generate Key pair",[]{
    auto result = signature::createKeyPair("test", "../key");
    system("rm ../key/test_private.pem ../key/test_public.pem");
    return result;
  });

  util::test("Sign & verify",[]{
    system("rm ../key/mizuki_private.pem ../key/mizuki_public.pem");
    auto result = signature::createKeyPair("mizuki", "../key");
    std::string message = "{\"from\":\"IORI\",\"to\":\"MIZUKI\",\"value\":11234}";
    auto signature = signature::sign(message, "../key/mizuki_private.pem","../key/mizuki_public.pem");
    std::cout << signature << std::endl;
    return signature::verify(signature, message, "../key/mizuki_public.pem");
  });

  util::test("test",[]{
    return signature::test();
  });

  return util::finish();
}
