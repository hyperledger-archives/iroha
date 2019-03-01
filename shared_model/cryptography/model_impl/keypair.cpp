/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <utility>

#include "cryptography/keypair.hpp"

#include "utils/string_builder.hpp"

namespace shared_model {
  namespace crypto {

    const Keypair::PublicKeyType &Keypair::publicKey() const {
      return public_key_;
    }

    const Keypair::PrivateKeyType &Keypair::privateKey() const {
      return private_key_;
    }

    bool Keypair::operator==(const Keypair &keypair) const {
      return publicKey() == keypair.publicKey()
          and privateKey() == keypair.privateKey();
    }

    std::string Keypair::toString() const {
      return detail::PrettyStringBuilder()
          .init("Keypair")
          .append("publicKey", publicKey().toString())
          .append("privateKey", privateKey().toString())
          .finalize();
    }

    Keypair *Keypair::clone() const {
      return new Keypair(publicKey(), privateKey());
    }

    Keypair::Keypair(Keypair::PublicKeyType public_key,
                     Keypair::PrivateKeyType private_key)
        : public_key_(std::move(public_key)), private_key_(std::move(private_key)) {}

  }  // namespace crypto
}  // namespace shared_model
