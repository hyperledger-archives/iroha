//
// Created by dainfos on 30.06.17.
//

#ifndef IROHA_DAO_CRYPTO_PROVIDER_STUB_HPP
#define IROHA_DAO_CRYPTO_PROVIDER_STUB_HPP

#include "dao_crypto_provider.hpp"
#include "transaction.hpp"

namespace iroha {
  namespace dao {

    class DaoCryptoProviderStub : public DaoCryptoProvider {
     public:
      bool verify(const iroha::dao::Transaction &tx) override {
        return true;
      }

      iroha::dao::Transaction sign(
          const iroha::dao::Transaction &tx) override{
        return iroha::dao::Transaction{};
      }

    };

  }
}

#endif  // IROHA_DAO_CRYPTO_PROVIDER_STUB_HPP
