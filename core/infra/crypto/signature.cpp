/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <algorithm>

#include <ed25519.h>

#include <crypto/signature.hpp>
#include <crypto/base64.hpp>
#include <crypto/hash.hpp>

namespace signature {

  template<typename T>
    std::unique_ptr<T[]> vector2UnsignedCharPointer(
    const std::vector<T> &vec
  ){
    std::unique_ptr<T[]> res(new T[sizeof(T)*vec.size()+1]);
    size_t pos = 0;
    auto* v = res.get();
    for(auto c : vec){
      v[pos++] = c;
    }
    v[pos] = '\0';
    return res;
  }

  template<typename T>
  std::vector<T> pointer2Vector(
    std::unique_ptr<T[]>&& array,
    size_t length
  ) {
      std::vector<T> res(length);
      res.assign(array.get(),array.get()+length);
      return res;
  }

  bool verify(
    const std::string &signature,
    const std::string &message,
    const std::string &publicKey) {
    return ed25519_verify(
      vector2UnsignedCharPointer(base64::decode(signature)).get(),
      reinterpret_cast<const unsigned char*>(message.c_str()),
      message.size(),
      vector2UnsignedCharPointer(base64::decode(publicKey)).get());
  }

  

  std::string sign(
    std::string message,
    KeyPair     keyPair
  ){
    std::unique_ptr<unsigned char[]> signature(new unsigned char[sizeof(unsigned char)*64]);
    ed25519_sign(
      signature.get(),
      reinterpret_cast<const unsigned char*>(message.c_str()),
      message.size(),
      vector2UnsignedCharPointer(keyPair.publicKey).get(),
      vector2UnsignedCharPointer(keyPair.privateKey).get()
    );
    return base64::encode(pointer2Vector(std::move(signature), 64));
  }

  std::string sign(
    std::string message,
    std::string publicKey_b64,
    std::string privateKey_b64
  ){
    std::unique_ptr<unsigned char[]> signatureRaw(new unsigned char[sizeof(unsigned char)*64]);
    ed25519_sign(
      signatureRaw.get(),
      reinterpret_cast<const unsigned char*>(message.c_str()),
      message.size(),
      vector2UnsignedCharPointer<unsigned char>(
        base64::decode(publicKey_b64)
      ).get(),
      vector2UnsignedCharPointer<unsigned char>(
        base64::decode(privateKey_b64)
      ).get()
    );
    return base64::encode(
      pointer2Vector(std::move(signatureRaw), 64)
    );
  }

  KeyPair generateKeyPair() {
    std::unique_ptr<unsigned char[]> publicKeyRaw(new unsigned char[sizeof(unsigned char)*32]);
    std::unique_ptr<unsigned char[]> privateKeyRaw(new unsigned char[sizeof(unsigned char)*64]);
    std::unique_ptr<unsigned char[]> seed(new unsigned char[sizeof(unsigned char)*32]);

    ed25519_create_seed(seed.get());
    ed25519_create_keypair(
      publicKeyRaw.get(),
      privateKeyRaw.get(),
      seed.get()
    );

    return KeyPair(
       pointer2Vector(std::move(publicKeyRaw), 32),
       pointer2Vector(std::move(privateKeyRaw), 64)
     );
  }
};  // namespace signature
