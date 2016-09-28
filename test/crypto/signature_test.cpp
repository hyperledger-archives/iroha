

#include "../../core/crypto/base64.hpp"
#include "../../core/crypto/signature.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(Signature, E){
  signature::KeyPair keyPair = signature::generateKeyPair();
  std::string nonce = "c0a5cca43b8aa79eb50e3464bc839dd6fd414fae0ddf928ca23dcebf8a8b8dd0";

  std::string signature = signature::sign(
    nonce,
    base64::encode(keyPair.publicKey),
    base64::encode(keyPair.privateKey)
  );

  ASSERT_TRUE(signature::verify(
    signature,
    nonce,
    base64::encode(keyPair.publicKey)));
}

TEST(Signature, keyPair){
  signature::KeyPair keyPair = signature::generateKeyPair();

  std::string nonce = "c0a5cca43b8aa79eb50e3464bc839dd6fd414fae0ddf928ca23dcebf8a8b8dd0";
  std::string signature_b64 = signature::sign(
    nonce,
    keyPair
  );

  ASSERT_TRUE(signature::verify(
    signature_b64,
    nonce,
    base64::encode(keyPair.publicKey))
  );
}

TEST(Signature, PrintkeyPair){
  signature::KeyPair keyPair = signature::generateKeyPair();
  std::cout << base64::encode(keyPair.publicKey) << std::endl;
  std::cout << base64::encode(keyPair.privateKey) << std::endl;
}
