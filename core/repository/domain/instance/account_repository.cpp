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

#include "../account_repository.hpp"
#include "common_repository.hpp"
#include <crypto/hash.hpp>
#include <repository/world_state_repository.hpp>
#include <transaction_builder/transaction_builder.hpp>
#include <util/convert_string.hpp>
#include <util/logger.hpp>

namespace common = ::repository::common;

const std::string NameSpaceID = "account repository";
const auto ValuePrefix = common::Prefix("Account::");

namespace repository {
namespace account {

namespace detail {
/********************************************************************************************
 * utils
 ********************************************************************************************/
std::string createAccountUuid(const std::string &publicKey) {
  /*
      account-uuid (Reason for account-uuid is url param can not use base64 in
     default,
      And accout-uuid is sha3-256(publicKey) )
  */
  return hash::sha3_256_hex(publicKey);
}
}

/********************************************************************************************
 * Add<Account>
 ********************************************************************************************/
std::string add(const std::string &publicKey, const std::string &name,
                const std::vector<std::string> &assets) {

  const auto allAssets = convert_string::to_string(assets);

  logger::explore(NameSpaceID) << "Add<Account> publicKey: " << publicKey
                               << " name: " << name << " assets: " << allAssets;

  const auto uuid = detail::createAccountUuid(publicKey);
  if (!exists(uuid)) {
    const auto account = txbuilder::createAccount(publicKey, name, assets);
    const auto strAccount = common::stringify<Api::Account>(account, ValuePrefix);
    logger::debug(NameSpaceID) << "Save key: " << uuid << " strAccount: \""
                               << strAccount << "\"";
    if (world_state_repository::add(uuid, strAccount)) {
      return uuid;
    }
  }

  return "";
}

/********************************************************************************************
 * Add<Asset, To<Account>>
 ********************************************************************************************/
bool attach(const std::string &uuid, const std::string &asset) {

  if (!exists(uuid)) {
    return false;
  }

  const auto strAccount = world_state_repository::find(uuid);


  for (int i=0; i<(int)asset.size(); i++) {
    assert(! (!isdigit(asset[i]) && !isalpha(asset[i])));
  }

  Api::Account account = common::parse<Api::Account>(strAccount, ValuePrefix);
  account.add_assets(asset);

  auto str = common::stringify<Api::Account>(account, ValuePrefix);
  if (world_state_repository::update(uuid, str)) {
    logger::explore(NameSpaceID) << "Add<Asset, To<Account>> uuid: " << uuid
                                 << "asset: " << asset;
    return true;
  }

  return false;
}

/********************************************************************************************
 * Update<Account>
 ********************************************************************************************/
bool update(const std::string &uuid, const std::string& name, const std::vector<std::string> &assets) {

  const auto allAssets = convert_string::stringifyVector(assets);

  logger::explore(NameSpaceID) << "Update<Account> uuid: " << uuid
                               << " name: " << name
                               << " assets: " << allAssets;

  if (exists(uuid)) {
    const auto rval = world_state_repository::find(uuid);
    auto account = common::parse<Api::Account>(rval, ValuePrefix);
    *account.mutable_name() = name;
    *account.mutable_assets() = txbuilder::Vector<std::string>(assets.begin(), assets.end());
    const auto strAccount = common::stringify<Api::Account>(account, ValuePrefix);
    if (world_state_repository::update(uuid, strAccount)) {
      logger::debug(NameSpaceID) << "Update strAccount: \"" << strAccount
                                 << "\"";
      return true;
    }
  }

  return false;
}
/********************************************************************************************
 * Remove<Account>
 ********************************************************************************************/
bool remove(const std::string &uuid) {
  if (exists(uuid)) {
    logger::explore(NameSpaceID) << "Remove<Account> uuid: " << uuid;
    return world_state_repository::remove(uuid);
  }
  return false;
}

/********************************************************************************************
 * find
 ********************************************************************************************/
Api::Account findByUuid(const std::string &uuid) {
  if (exists(uuid)) {
    const auto strAccount = world_state_repository::find(uuid);
    logger::explore(NameSpaceID + "findByUuid") << "";
    return common::parse<Api::Account>(strAccount, ValuePrefix);
  }
  return Api::Account();
}

bool exists(const std::string &uuid) {
  const auto result = world_state_repository::exists(uuid);
  logger::explore(NameSpaceID + "::exists") << (result ? "true" : "false");
  return result;
}
}
}