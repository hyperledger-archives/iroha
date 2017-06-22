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
#ifndef IROHA_QUERY_API_HPP
#define IROHA_QUERY_API_HPP
#include <dao.hpp>
#include <vector>
namespace iroha {

namespace ametsuchi {

class QueryApi {
  //TODO:
  virtual iroha::dao::Account get_account() = 0;

  virtual iroha::dao::Domain get_domain() = 0;

  virtual iroha::dao::Asset get_asset() = 0;

  virtual iroha::dao::Wallet get_wallet() = 0;

  virtual std::vector<iroha::dao::Wallet> get_account_wallets() = 0;


};

}
}

#endif //IROHA_QUERY_API_HPP
