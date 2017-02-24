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
#include <transaction_builder/helper/create_objects_helper.hpp>
#include <util/convert_string.hpp>
#include <util/logger.hpp>

#include "jni_constants.hpp"
#include "test_repository_DomainRepository.h"

const std::string NameSpaceID = "domain repo jni";

using virtual_machine::jvm::JavaMakeBoolean;
using virtual_machine::jvm::JavaMakeMap;
using virtual_machine::jvm::convertJavaStringArrayRelease;
using virtual_machine::jvm::convertJavaHashMapValueString;
using virtual_machine::jvm::convertJavaHashMapValueHashMap;
using virtual_machine::jvm::JavaMakeStringArray;

/***************************************************************************************
 * Account
 ***************************************************************************************/
JNIEXPORT jstring JNICALL Java_test_repository_DomainRepository_accountAdd(
    JNIEnv *env, jclass, jstring publicKey_, jstring name_,
    jobjectArray assets_) {
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

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_accountAttach(
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

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_accountUpdate(
    JNIEnv *env, jclass, jstring uuid_, jobjectArray assets_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);

  const auto uuid = std::string(uuidCString);
  const auto assets = convertJavaStringArrayRelease(env, assets_);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug(NameSpaceID + "::accountUpdate")
      << " uuid: " << uuid << ", assets: " << convert_string::to_string(assets);

  return JavaMakeBoolean(env, repository::account::update(uuid, assets));
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_accountRemove(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::accountRemove") << " uuid: " << uuid;
  return JavaMakeBoolean(env, repository::account::remove(uuid));
}

JNIEXPORT jobject JNICALL
Java_test_repository_DomainRepository_accountInfoFindByUuid(JNIEnv *env, jclass,
                                                            jstring uuid_) {
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
Java_test_repository_DomainRepository_accountValueFindByUuid(JNIEnv *env,
                                                             jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  Api::Account account = repository::account::findByUuid(uuid);

  const auto assets = txbuilder::createStandardVector(account.assets());

  logger::debug(NameSpaceID + "::accountValueFindByUuid")
        << "value: " << convert_string::to_string(assets);

  return JavaMakeStringArray(env, assets);
}

/***************************************************************************************
 * Asset
 ***************************************************************************************/

/**********************************************************************
 * txbuilder::Map<std::string, Api::BaseObject>
 * <-> HashMap<String, HashMap<String, String>>
 **********************************************************************/
namespace detail {

const std::string ValueTypeString = "string";
const std::string ValueTypeInt = "int";
const std::string ValueTypeBoolean = "boolean";
const std::string ValueTypeDouble = "double";

txbuilder::Map convertAssetValueMap(JNIEnv *env, jobject value_) {

  txbuilder::Map ret;

  std::map<std::string, std::map<std::string, std::string>> valueMapSM =
      convertJavaHashMapValueHashMap(env, value_);

  for (auto &e : valueMapSM) {
    const auto &key = e.first;
    auto &baseObjectMap = e.second;
    const auto valueType = baseObjectMap["type"];
    const auto content = baseObjectMap["value"];
    if (valueType == ValueTypeString) {
      ret.emplace(key, txbuilder::createValueString(content));
    } else if (valueType == ValueTypeInt) {
      try {
        ret.emplace(key, txbuilder::createValueInt(std::stoi(content)));
      } catch (std::invalid_argument) {
        throw "Value type mismatch: expected int";
      }
    } else if (valueType == ValueTypeBoolean) {
      auto boolStr = content;
      std::transform(boolStr.begin(), boolStr.end(), boolStr.begin(),
                     ::tolower);
      ret.emplace(key, txbuilder::createValueBool(boolStr == "true"));
    } else if (valueType == ValueTypeDouble) {
      try {
        ret.emplace(key, txbuilder::createValueDouble(std::stod(content)));
      } catch (std::invalid_argument) {
        throw "Value type mismatch: expected double";
      }
    } else {
      throw "Unknown value type";
    }
  }

  return ret;
}

std::map<std::string, std::string>
convertBaseObjectToMapString(const Api::BaseObject &value) {

  std::map<std::string, std::string> baseObjectMap;

  switch (value.value_case()) {
  case Api::BaseObject::kValueString:
    baseObjectMap.emplace("type", ValueTypeString);
    baseObjectMap.emplace("value", value.valuestring());
    break;
  case Api::BaseObject::kValueInt:
    baseObjectMap.emplace("type", ValueTypeInt);
    baseObjectMap.emplace("value", std::to_string(value.valueint()));
    break;
  case Api::BaseObject::kValueBoolean:
    baseObjectMap.emplace("type", ValueTypeBoolean);
    baseObjectMap.emplace("value", std::string(value.valueboolean() ? "true" : "false"));
    break;
  case Api::BaseObject::kValueDouble:
    baseObjectMap.emplace("type", ValueTypeDouble);
    baseObjectMap.emplace("value", std::to_string(value.valuedouble()));
    break;
  default:
    throw "Invalid type";
  }

  return baseObjectMap;
}

jobject JavaMakeAssetValueMap(JNIEnv *env, const txbuilder::Map& value) {

  // txbuilder::Map<string, Api::BaseObject> -> map<string, map<string, string>>
  std::map<std::string, std::map<std::string, std::string>> cppMapInMap;
  for (const auto &e : value) {
    const auto &key = e.first;
    cppMapInMap.emplace(key, convertBaseObjectToMapString(e.second));
  }

  // map<string, map<string, string>>
  // -> HashMap<String, HashMap<String,String>>
  return JavaMakeMap(env, cppMapInMap);
}
} // namespace detail

JNIEXPORT jstring JNICALL Java_test_repository_DomainRepository_assetAdd(
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

  const auto value = detail::convertAssetValueMap(env, value_);

  logger::debug(NameSpaceID + "::assetAdd")
      << "domainId: " << domain << ", assetName: " << name
      << ", smartContractName: " << smartContractName;

  const auto ret =
      repository::asset::add(domain, name, value, smartContractName);

  return env->NewStringUTF(ret.c_str());
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_assetUpdate(
    JNIEnv *env, jclass, jstring uuid_, jobject value_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  const auto value = detail::convertAssetValueMap(env, value_);

  return JavaMakeBoolean(env, repository::asset::update(uuid, value));
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_assetRemove(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  return JavaMakeBoolean(env, repository::asset::remove(uuid));
}

JNIEXPORT jobject JNICALL
Java_test_repository_DomainRepository_assetInfoFindByUuid(JNIEnv *env, jclass,
                                                          jstring uuid_) {
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
      << "params[DomainIdTag]: \"value\":" << params[DomainIdTag]
      << ", "
      << "params[AssetNameTag]: \"value\":" << params[AssetNameTag]
      << ", "
      << "params[SmartContractNameTag]: \"value\":" << params[SmartContractNameTag];

  return JavaMakeMap(env, params);
}

JNIEXPORT jobject JNICALL
Java_test_repository_DomainRepository_assetValueFindByUuid(JNIEnv *env, jclass,
                                                           jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug(NameSpaceID + "::assetValueFindByUuid") << "uuid: " << uuid;

  auto asset = repository::asset::findByUuid(uuid);

  ::txbuilder::Map value(asset.value().begin(), asset.value().end());
  // std::map<std::string, Api::BaseObject>
  // -> std::map<std::string, std::map<std::string, std::string>>
  std::map<std::string, std::map<std::string, std::string>> params;
  for (auto&& e: value) {
    params.emplace(e.first, detail::convertBaseObjectToMapString(e.second));
  }

  std::string paramsStr = "{";
  for (auto&& e: params) {
    paramsStr += "{" + e.first + ",";
    for (auto&& u: e.second) {
      paramsStr += "{" + u.first + "," + u.second + "},";
    }
    paramsStr += "},";
  }
  paramsStr += "}";

  logger::debug(NameSpaceID + "::assetValueFindByUuid") << "value: " << paramsStr;

  return JavaMakeMap(env, params);
}