

#include "../../core/crypto/signature.hpp"
#include "../../core/crypto/base64.hpp"

#include <tuple>
#include <iostream>
#include <gtest/gtest.h>

#include <cstring>

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

/*
TEST(Signature, Verify){
  std::string publicKey = "5P97anJnp6H6Ul51dlg8k3euFRgb1jtcinwCHNTS2x8=";
  std::string signature = "N3kWmMbroZc82Uug//KaT+gNPyceV4odVS053lC2b5ohr5i4sLKOQELfVm2tcQKkgFy36vyxhuQQgBlVN7sOCA==";
  std::string nonce = "c0a5cca43b8aa79eb50e3464bc839dd6fd414fae0ddf928ca23dcebf8a8b8dd0";
  ASSERT_TRUE(signature::verify(
    signature,
    nonce,
    publicKey)
  );
}
*/
