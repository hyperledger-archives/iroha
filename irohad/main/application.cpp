/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "application.hpp"

#include <ametsuchi/ametsuchi_stub.hpp>
#include "dao/dao_crypto_provider_stub.hpp"
#include "dao/dao_hash_provider_impl.hpp"

namespace iroha {

  // TODO add initialization of ametsuchi and peer service
  Irohad::Irohad()
      : ametsuchi(*initialize_ametsuchi()),
        cryptoProvider(*initialize_crypto_provider()),
        hashProvider(*initialize_hash_provider()) {}

  std::unique_ptr<dao::HashProvider<32>> Irohad::initialize_hash_provider() {
    return std::make_unique<dao::HashProviderImpl>();
  }
  std::unique_ptr<dao::DaoCryptoProvider> Irohad::initialize_crypto_provider() {
    return std::make_unique<dao::DaoCryptoProviderStub>();
  }
  std::unique_ptr<ametsuchi::Ametsuchi> Irohad::initialize_ametsuchi() {
    return std::make_unique<ametsuchi::AmetsuchiStub>();
  }
}