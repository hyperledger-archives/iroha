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

#include "../asset_repository.hpp"
#include "common_repository.hpp"
#include <crypto/hash.hpp>
#include <repository/world_state_repository.hpp>
#include <transaction_builder/transaction_builder.hpp>
#include <util/exception.hpp>
#include <util/logger.hpp>

namespace common = ::repository::common;

const std::string NameSpaceID = "asset repository";
const auto ValuePrefix = common::Prefix("Asset::");

namespace repository {
namespace asset {

namespace detail {
std::string createAssetUuid(const std::string &domain,
                            const std::string &name) {
  return hash::sha3_256_hex(domain + "@" + name);
}
}

/********************************************************************************************
 * Add<Asset>
 ********************************************************************************************/
std::string add(const std::string &domain, const std::string &name,
                const txbuilder::Map &value,
                const std::string &smartContractName) {

  logger::explore(NameSpaceID) << "Add<Asset> domainId: " << domain
                               << " assetName: " << name
                               << " assetValue: " << txbuilder::stringify(value)
                               << " smartContractName: " << smartContractName;

  const auto uuid = detail::createAssetUuid(domain, name);

  if (not exists(uuid)) {
    const auto strAsset = common::stringify<Api::Asset>(
        txbuilder::createAsset(domain, name, value, smartContractName),
        ValuePrefix);
    if (world_state_repository::add(uuid, strAsset)) {
      return uuid;
    }
  }

  return "";
}

/********************************************************************************************
 * Update<Asset>
 ********************************************************************************************/
bool update(const std::string &uuid, const txbuilder::Map &value) {
  if (exists(uuid)) {
    const auto rval = world_state_repository::find(uuid);
    logger::explore(NameSpaceID) << "Update<Asset> uuid: " << uuid
                                 << txbuilder::stringify(value);
    auto asset = common::parse<Api::Asset>(rval, ValuePrefix);
    *asset.mutable_value() =
        google::protobuf::Map<std::string, Api::BaseObject>(value.begin(),
                                                            value.end());
    const auto strAsset = common::stringify<Api::Asset>(asset, ValuePrefix);
    return world_state_repository::update(uuid, strAsset);
  }
  return false;
}

/********************************************************************************************
 * Remove<Asset>
 ********************************************************************************************/
bool remove(const std::string &uuid) {
  if (exists(uuid)) {
    logger::explore(NameSpaceID) << "Remove<Asset> uuid: " << uuid;
    return world_state_repository::remove(uuid);
  }
  return false;
}

/********************************************************************************************
 * find
 ********************************************************************************************/
std::vector<Api::Asset> findAll(const std::string &uuid) {
  /* Use world_state_repository::findByPrefix()*/
  throw exception::NotImplementedException(__func__, __FILE__);
}

Api::Asset findByUuid(const std::string &uuid) {

  logger::explore(NameSpaceID + "::findByUuid") << "";
  auto strAsset = world_state_repository::find(uuid);
  if (!strAsset.empty()) {
    return common::parse<Api::Asset>(strAsset, ValuePrefix);
  }

  return Api::Asset();
}

bool exists(const std::string &uuid) {
  const auto result = world_state_repository::exists(uuid);
  logger::explore(NameSpaceID + "::exists") << (result ? "true" : "false");
  return result;
}
}
}
