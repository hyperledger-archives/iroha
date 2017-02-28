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
#include <assert.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <infra/protobuf/api.pb.h>
#include <infra/virtual_machine/jvm/java_data_structure.hpp>
#include <repository/domain/account_repository.hpp>
#include <repository/domain/asset_repository.hpp>
#include <repository/domain/domain_repository.hpp>
#include <repository/domain/simple_asset_repository.hpp>
#include <transaction_builder/helper/create_objects_helper.hpp>
#include <util/convert_string.hpp>
#include <util/logger.hpp>

#include "jni_constants.hpp"
#include "repository_Repository.h"

const std::string NameSpaceID = "domain repo jni";

using virtual_machine::jvm::JavaMakeBoolean;
using virtual_machine::jvm::JavaMakeMap;
using virtual_machine::jvm::convertJavaStringArrayRelease;
using virtual_machine::jvm::convertJavaHashMapValueString;
using virtual_machine::jvm::convertJavaHashMapValueHashMap;
using virtual_machine::jvm::convertAssetValueHashMap;
using virtual_machine::jvm::convertSimpleAssetValueHashMap;
using virtual_machine::jvm::convertBaseObjectToMapString;
using virtual_machine::jvm::JavaMakeStringArray;

/***************************************************************************************
 * Account
 ***************************************************************************************/
JNIEXPORT jstring JNICALL
Java_repository_Repository_accountAdd(JNIEnv *env, jclass, jstring publicKey_,
                                      jstring name_, jobjectArray assets_) {
  const char *publicKeyCString = env->GetStringUTFChars(publicKey_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);

  const auto publicKey = std::string(publicKeyCString);
  const auto name = std::string(nameCString);
  const auto assets = convertJavaStringArrayRelease(env, assets_);

  env->ReleaseStringUTFChars(publicKey_, publicKeyCString);
  env->ReleaseStringUTFChars(name_, nameCString);

  logger::debug(NameSpaceID + "::accountAdd")
      << "key: " << publicKey << " name: " << name
      << " assets: " << convert_string::to_string(assets);

  const auto ret = repository::account::add(publicKey, name, assets);
  return env->NewStringUTF(ret.c_str());
}

JNIEXPORT jobject JNICALL Java_repository_Repository_accountAttach(
    JNIEnv *env, jclass, jstring uuid_, jstring asset_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const char *assetCString = env->GetStringUTFChars(asset_, 0);

  const auto uuid = std::string(uuidCString);
  const auto asset = std::string(assetCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);
  env->ReleaseStringUTFChars(asset_, assetCString);

  logger::debug(NameSpaceID + "::accountAttach") << "uuid: " << uuid
                                                 << ", asset: " << asset;

  return JavaMakeBoolean(env, repository::account::attach(uuid, asset));
}

JNIEXPORT jobject JNICALL Java_repository_Repository_accountUpdate(
    JNIEnv *env, jclass, jstring uuid_, jstring name_, jobjectArray assets_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);

  const auto uuid = std::string(uuidCString);
  const auto name = std::string(nameCString);
  const auto assets = convertJavaStringArrayRelease(env, assets_);

  env->ReleaseStringUTFChars(uuid_, uuidCString);
  env->ReleaseStringUTFChars(name_, nameCString);

  logger::debug(NameSpaceID + "::accountUpdate")
      << " uuid: " << uuid << ", name: " << name
      << ", assets: " << convert_string::to_string(assets);

  return JavaMakeBoolean(env, repository::account::update(uuid, name, assets));
}

JNIEXPORT jobject JNICALL
Java_repository_Repository_accountRemove(JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::accountRemove") << " uuid: " << uuid;
  return JavaMakeBoolean(env, repository::account::remove(uuid));
}

JNIEXPORT jobject JNICALL Java_repository_Repository_accountInfoFindByUuid(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  Api::Account account = repository::account::findByUuid(uuid);

  using jni_constants::PublicKeyTag;
  using jni_constants::AccountNameTag;

  std::map<std::string, std::string> params;
  {
    params[PublicKeyTag] = account.publickey();
    params[AccountNameTag] = account.name();
  }

  logger::debug(NameSpaceID + "::accountInfoFindByUuid")
      << "params[PublicKeyTag]: " << params[PublicKeyTag] << ", "
      << "params[AccountNameTag]: " << params[AccountNameTag];

  return JavaMakeMap(env, params);
}

JNIEXPORT jobjectArray JNICALL
Java_repository_Repository_accountValueFindByUuid(JNIEnv *env, jclass,
                                                  jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  Api::Account account = repository::account::findByUuid(uuid);

  const auto assets = txbuilder::createStandardVector(account.assets());

  logger::debug(NameSpaceID + "::accountValueFindByUuid")
      << "value: " << convert_string::to_string(assets);

  return JavaMakeStringArray(env, assets);
}

JNIEXPORT jobject JNICALL
Java_repository_Repository_accountExists(JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::accountExists") << " uuid: " << uuid;
  return JavaMakeBoolean(env, repository::account::exists(uuid));
}

/***************************************************************************************
 * Asset
 ***************************************************************************************/

JNIEXPORT jstring JNICALL Java_repository_Repository_assetAdd(
    JNIEnv *env, jclass, jstring domain_, jstring name_, jobject value_,
    jstring smartContractName_) {

  const char *domainCString = env->GetStringUTFChars(domain_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);
  const char *smartContractNameCString =
      env->GetStringUTFChars(smartContractName_, 0);

  const auto domain = std::string(domainCString);
  const auto name = std::string(nameCString);
  const auto smartContractName = std::string(smartContractNameCString);

  env->ReleaseStringUTFChars(domain_, domainCString);
  env->ReleaseStringUTFChars(name_, nameCString);
  env->ReleaseStringUTFChars(smartContractName_, smartContractNameCString);

  const auto value = convertAssetValueHashMap(env, value_);

  logger::debug(NameSpaceID + "::assetAdd")
      << "domainId: " << domain << ", assetName: " << name
      << ", smartContractName: " << smartContractName;

  const auto ret =
      repository::asset::add(domain, name, value, smartContractName);

  return env->NewStringUTF(ret.c_str());
}

JNIEXPORT jobject JNICALL Java_repository_Repository_assetUpdate(
    JNIEnv *env, jclass, jstring uuid_, jobject value_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  const auto value = convertAssetValueHashMap(env, value_);

  return JavaMakeBoolean(env, repository::asset::update(uuid, value));
}

JNIEXPORT jobject JNICALL
Java_repository_Repository_assetRemove(JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  return JavaMakeBoolean(env, repository::asset::remove(uuid));
}

JNIEXPORT jobject JNICALL Java_repository_Repository_assetInfoFindByUuid(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug(NameSpaceID + "::assetInfoFindByUuid") << "uuid: " << uuid;

  auto asset = repository::asset::findByUuid(uuid);

  using jni_constants::DomainIdTag;
  using jni_constants::AssetNameTag;
  using jni_constants::SmartContractNameTag;

  std::map<std::string, std::string> params;
  {
    params[DomainIdTag] = asset.domain();
    params[AssetNameTag] = asset.name();
    params[SmartContractNameTag] = asset.smartcontractname();
  }

  logger::debug(NameSpaceID + "::assetInfoFindByUuid")
      << "params[DomainIdTag]: \"value\":" << params[DomainIdTag] << ", "
      << "params[AssetNameTag]: \"value\":" << params[AssetNameTag] << ", "
      << "params[SmartContractNameTag]: \"value\":"
      << params[SmartContractNameTag];

  return JavaMakeMap(env, params);
}

JNIEXPORT jobject JNICALL Java_repository_Repository_assetValueFindByUuid(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug(NameSpaceID + "::assetValueFindByUuid") << "uuid: " << uuid;

  auto asset = repository::asset::findByUuid(uuid);

  ::txbuilder::Map value(asset.value().begin(), asset.value().end());
  // std::map<std::string, Api::BaseObject>
  // -> std::map<std::string, std::map<std::string, std::string>>
  std::map<std::string, std::map<std::string, std::string>> params;
  for (auto &&e : value) {
    params.emplace(e.first, convertBaseObjectToMapString(e.second));
  }

  std::string paramsStr = "{";
  for (auto &&e : params) {
    paramsStr += "{" + e.first + ",";
    for (auto &&u : e.second) {
      paramsStr += "{" + u.first + "," + u.second + "},";
    }
    paramsStr += "},";
  }
  paramsStr += "}";

  logger::debug(NameSpaceID + "::assetValueFindByUuid") << "value: "
                                                        << paramsStr;

  return JavaMakeMap(env, params);
}

JNIEXPORT jobject JNICALL
Java_repository_Repository_assetExists(JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::assetExists") << " uuid: " << uuid;
  return JavaMakeBoolean(env, repository::asset::exists(uuid));
}

/***************************************************************************************
 * SimpleAsset
 ***************************************************************************************/
JNIEXPORT jstring JNICALL Java_repository_Repository_simpleAssetAdd(
    JNIEnv *env, jclass, jstring domain_, jstring name_, jobject value_,
    jstring smartContractName_) {
  const char *domainCString = env->GetStringUTFChars(domain_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);
  const char *smartContractNameCString =
      env->GetStringUTFChars(smartContractName_, 0);

  const auto domain = std::string(domainCString);
  const auto name = std::string(nameCString);
  const auto smartContractName = std::string(smartContractNameCString);

  env->ReleaseStringUTFChars(domain_, domainCString);
  env->ReleaseStringUTFChars(name_, nameCString);
  env->ReleaseStringUTFChars(smartContractName_, smartContractNameCString);

  const Api::BaseObject value = convertSimpleAssetValueHashMap(env, value_);

  logger::debug(NameSpaceID + "::simpleAssetAdd")
      << "domainId: " << domain << ", name: " << name
      << ", smartContractName: " << smartContractName;

  const auto ret =
      repository::simple_asset::add(domain, name, value, smartContractName);

  return env->NewStringUTF(ret.c_str());
}

JNIEXPORT jobject JNICALL Java_repository_Repository_simpleAssetUpdate(
    JNIEnv *env, jclass, jstring uuid_, jobject value_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  const auto value = convertSimpleAssetValueHashMap(env, value_);

  return JavaMakeBoolean(env, repository::simple_asset::update(uuid, value));
}

JNIEXPORT jobject JNICALL Java_repository_Repository_simpleAssetRemove(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  return JavaMakeBoolean(env, repository::simple_asset::remove(uuid));
}

JNIEXPORT jobject JNICALL Java_repository_Repository_simpleAssetInfoFindByUuid(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug(NameSpaceID + "::simpleAssetInfoFindByUuid") << "uuid: "
                                                             << uuid;

  auto simpleAsset = repository::simple_asset::findByUuid(uuid);

  using jni_constants::DomainIdTag;
  using jni_constants::AssetNameTag;
  using jni_constants::SmartContractNameTag;

  std::map<std::string, std::string> params;
  {
    params[DomainIdTag] = simpleAsset.domain();
    params[AssetNameTag] = simpleAsset.name();
    params[SmartContractNameTag] = simpleAsset.smartcontractname();
  }

  logger::debug(NameSpaceID + "::simpleAssetInfoFindByUuid")
      << "params[DomainIdTag]: \"value\":" << params[DomainIdTag] << ", "
      << "params[AssetNameTag]: \"value\":" << params[AssetNameTag] << ", "
      << "params[SmartContractNameTag]: \"value\":"
      << params[SmartContractNameTag];

  return JavaMakeMap(env, params);
}

JNIEXPORT jobject JNICALL Java_repository_Repository_simpleAssetValueFindByUuid(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug(NameSpaceID + "::simpleAssetValueFindByUuid") << "uuid: "
                                                              << uuid;

  auto simpleAsset = repository::simple_asset::findByUuid(uuid);
  return JavaMakeMap(env, convertBaseObjectToMapString(simpleAsset.value()));
}

JNIEXPORT jobject JNICALL Java_repository_Repository_simpleAssetExists(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::simpleAssetExists") << " uuid: " << uuid;
  return JavaMakeBoolean(env, repository::simple_asset::exists(uuid));
}

/***************************************************************************************
 * Domain
 ***************************************************************************************/
JNIEXPORT jstring JNICALL Java_repository_Repository_domainAdd(
    JNIEnv *env, jclass, jstring ownerPublicKey_, jstring name_) {

  const char *ownerPublicKeyCString =
      env->GetStringUTFChars(ownerPublicKey_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);

  const auto ownerPublicKey = std::string(ownerPublicKeyCString);
  const auto name = std::string(nameCString);

  env->ReleaseStringUTFChars(ownerPublicKey_, ownerPublicKeyCString);
  env->ReleaseStringUTFChars(name_, nameCString);

  logger::debug(NameSpaceID + "::domainAdd")
      << "ownerPublicKey: " << ownerPublicKey_ << ", name: " << name_;

  const auto ret = repository::domain::add(ownerPublicKey, name);

  return env->NewStringUTF(ret.c_str());
}

JNIEXPORT jobject JNICALL Java_repository_Repository_domainUpdate(
    JNIEnv *env, jclass, jstring uuid_, jstring name_) {

  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);

  const auto uuid = std::string(uuidCString);
  const auto name = std::string(nameCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);
  env->ReleaseStringUTFChars(name_, nameCString);

  logger::debug(NameSpaceID + "::domainUpdate") << "uuid: " << uuid_
                                                << ", name: " << name_;

  return JavaMakeBoolean(env, repository::domain::update(uuid, name));
}

JNIEXPORT jobject JNICALL
Java_repository_Repository_domainRemove(JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::domainRemove") << "uuid: " << uuid;
  return JavaMakeBoolean(env, repository::domain::remove(uuid));
}

JNIEXPORT jobject JNICALL Java_repository_Repository_domainFindByUuid(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::domainFindByUuid") << "uuid: " << uuid;
  auto domain = repository::domain::findByUuid(uuid);
  std::map<std::string, std::string> domainMap;
  {
    domainMap[jni_constants::OwnerPublicKeyTag] = domain.ownerpublickey();
    domainMap[jni_constants::DomainNameTag] = domain.name();
  }
  return JavaMakeMap(env, domainMap);
}

JNIEXPORT jobject JNICALL
Java_repository_Repository_domainExists(JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::domainExists") << " uuid: " << uuid;
  return JavaMakeBoolean(env, repository::domain::exists(uuid));
}

/***************************************************************************************
 * Peer
 ***************************************************************************************/
