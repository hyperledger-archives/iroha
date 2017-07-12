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

#include <main/application.hpp>

Irohad::Irohad():
  context(new Context())
{}

void Irohad::run(){
//  iroha::Irohad irohad;
//  iroha::ametsuchi::StorageImpl ametsuchi;

  // TODO replace with actual public private keys
  auto seed = iroha::create_seed("some passphrase");
  auto keypair = iroha::create_keypair(seed);
  iroha::model::ModelCryptoProviderImpl crypto_provider(keypair.privkey, keypair.pubkey);

  iroha::validation::StatelessValidatorImpl stateless_validator(crypto_provider);
  iroha::validation::StatefulValidatorStub stateful_validator;
  iroha::validation::ChainValidatorStub chain_validator;
  iroha::ordering::OrderingServiceStub ordering_service;
  iroha::consensus::ConsensusServiceStub consensus_service;
  iroha::network::PeerCommunicationServiceStub peer_communication_service(
    ordering_service,
    consensus_service);
  iroha::torii::TransactionProcessorStub tp(
    stateless_validator,
    crypto_provider
  );
//  iroha::torii::QueryProcessorStub qp(ametsuchi, ametsuchi);

//  iroha::torii::ToriiStub torii(tp, qp);

}
