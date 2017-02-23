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
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <infra/smart_contract/jvm/java_virtual_machine.hpp>
#include <repository/domain/account_repository.hpp>
#include <repository/domain/asset_repository.hpp>
#include <transaction_builder/helper/create_objectrs_helper.hpp>
#include <util/logger.hpp>

#include "jni_constants.hpp"
#include "test_repository_DomainRepository.h"

const std::string NameSpaceID = "domain repo jni";

/***************************************************************************************
 * util
 **************************************************************************************/
namespace detail {
jobject JavaMakeBoolean(JNIEnv *env, jboolean value) {
  jclass booleanClass = env->FindClass("java/lang/Boolean");
  jmethodID methodID = env->GetMethodID(booleanClass, "<init>", "(Z)V", false);
  return env->NewObject(booleanClass, methodID, value);
}

std::vector<std::string> convertStringArrayRelease(jobjectArray javaArray_) {
  std::vector<std::string> ret;
  const int arraySize = env->GetArrayLength(javaArray_);
  for (int i = 0; i < arraySize; i++) {
    jstring elementString_ =
        (jstring)(env->GetObjectArrayElement(javaArray_, i));
    const char *elementCString = env->GetStringUTFChars(elementString, 0);
    ret.push_back(std::string(elementCString));
    env->ReleaseStringUTFChars(elementString_, elementCString);
  }
  return ret;
}

// ref: https://android.googlesource.com/platform/frameworks/base.git/+/a3804cf77f0edd93f6247a055cdafb856b117eec/media/jni/android_media_MediaMetadataRetriever.cpp
std::unordered_map<std::string, std::string>
convertHashMap(JNIEnv *env, jobject hashMapObj_) {
  /*
      Map<String, String> map = ...
      for (Map.Entry<String, String> entry : map.entrySet())
      {
          System.out.println(entry.getKey() + "/" + entry.getValue());
      }
   */
  jclass mapClass = env->FindClass("java/util/Map");
  if (mapClass == nullptr) {
    return {};
  }
  jmethodID entrySet =
      env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
  if (entrySet == nullptr) {
    return {};
  }
  jobject set = env->CallObjectMethod(hashMapObj_, entrySet);
  if (set == nullptr) {
    return {};
  }
  // Obtain an iterator over the Set
  jclass setClass = env->FindClass("java/util/Set");
  if (setClass == nullptr) {
    return {};
  }
  jmethodID iterator =
      env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
  if (iterator == nullptr) {
    return {};
  }
  jobject iter = env->CallObjectMethod(set, iterator);
  if (iter == nullptr) {
    return {};
  }
  // Get the Iterator method IDs
  jclass iteratorClass = env->FindClass("java/util/Iterator");
  if (iteratorClass == nullptr) {
    return {};
  }
  jmethodID hasNext = env->GetMethodID(iteratorClass, "hasNext", "()Z");
  if (hasNext == nullptr) {
    return {};
  }
  jmethodID next =
      env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
  if (next == nullptr) {
    return {};
  }
  // Get the Entry class method IDs
  jclass entryClass = env->FindClass("java/util/Map$Entry");
  if (entryClass == nullptr) {
    return {};
  }
  jmethodID getKey =
      env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
  if (getKey == nullptr) {
    return {};
  }
  jmethodID getValue =
      env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
  if (getValue == nullptr) {
    return {};
  }

  std::unordered_map<std::string, std::string> ret;

  // Iterate over the entry Set
  while (env->CallBooleanMethod(iter, hasNext)) {
    jobject entry = env->CallObjectMethod(iter, next);
    jstring key = (jstring)env->CallObjectMethod(entry, getKey);
    jstring value = (jstring)env->CallObjectMethod(entry, getValue);
    const char *keyStr = env->GetStringUTFChars(key, nullptr);
    if (!keyStr) { // Out of memory
      return {};
    }
    const char *valueStr = env->GetStringUTFChars(value, nullptr);
    if (!valueStr) { // Out of memory
      env->ReleaseStringUTFChars(key, keyStr);
      return {};
    }

    ret[std::string(keyStr)] = std::string(valueStr);

    env->DeleteLocalRef(entry);
    env->ReleaseStringUTFChars(key, keyStr);
    env->DeleteLocalRef(key);
    env->ReleaseStringUTFChars(value, valueStr);
    env->DeleteLocalRef(value);
  }
  return ret;
}
}

/***************************************************************************************
 * Account
 ***************************************************************************************/
JNIEXPORT jstring JNICALL Java_test_repository_DomainRepository_accountAdd(
    JNIEnv *env, jclass, jstring publicKey_, jstring name_,
    jobjectArray assets_) {
  const char *publicKeyCString = env->GetStringUTFChars(publicKey_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);

  const auto publicKey = std::string(publicKeyCString);
  const auto alias = std::string(aliasCString);
  const auto assets = detail::convertStringArrayRelease(assets_);

  env->ReleaseStringUTFChars(publicKey_, publicKeyCString);
  env->ReleaseStringUTFChars(alias_, aliasCString);

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
  env->ReleaseStringUTFChars(assetName_, assetCString);

  logger::debug(NameSpaceID + "::accountAttach") << "uuid: " << uuid
                                                 << ", asset: " << asset;

  return detail::JavaMakeBoolean(env, repository::account::attach(uuid, asset));
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_accountUpdate(
    JNIEnv *env, jclass, jstring uuid_, jobjectArray assets_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);

  const auto uuid = std::string(uuidCString);
  const auto assets = detail::convertStringArrayRelease(assets_);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug(NameSpaceID + "::accountUpdate")
      << " uuid: " << uuid << ", assets: " << convert_string::to_string(assets);

  return detail::JavaMakeBoolean(env,
                                 repository::account::update(uuid, assets));
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_accountRemove(
    JNIEnv *env, jclass, jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);
  env->ReleaseStringUTFChars(uuid_, uuidCString);
  logger::debug(NameSpaceID + "::accountRemove") << " uuid: " << uuid;
  return detail::JavaMakeBoolean(env, repository::account::remove(uuid));
}

JNIEXPORT jobject JNICALL
Java_test_repository_DomainRepository_accountFindByUuid(JNIEnv *env, jclass,
                                                        jstring uuid_) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  Api::Account account = repository::account::findByUuid(uuid);

  using jni_constants::PublicKeyTag;
  using jni_constants::AccountNameTag;
  using jni_constants::AssetsTag;

  std::unordered_map<std::string, std::string> params;
  {
    params[PublicKeyTag] = account.publickey();
    params[AccountNameTag] = account.name();
    params[AssetsTag] = ::txbuilder::createStandardVector(account.assets());
  }

  logger::debug(NameSpaceID + "::accountFindByUuid")
      << "params[PublicKeyTag]: " << params[PublicKeyTag] << ", "
      << "params[AccountNameTag]: " << params[AccountNameTag] << ", "
      << "params[AssetsTag]: " << params[AssetsTag];

  return smart_contract::JavaMakeMap(env, params);
}

/***************************************************************************************
 * Asset
 ***************************************************************************************/
JNIEXPORT jstring JNICALL Java_test_repository_DomainRepository_assetAdd(
    JNIEnv *env, jclass, jstring domain_, jstring name_, jobject value_,
    jstring smartContract_) {
  const char *domainCString = env->GetStringUTFChars(domain_, 0);
  const char *nameCString = env->GetStringUTFChars(name_, 0);
  const char *smartContractCString = env->GetStringUTFChars(smartContract_, 0);

  const auto domain = std::string(domainCString);
  const auto name = std::string(nameCString);
  const auto smartContract = std::string(smartContractCString);

  env->ReleaseStringUTFChars(domainId_, domainIdCString);
  env->ReleaseStringUTFChars(assetName_, assetNameCString);
  env->ReleaseStringUTFChars(smartContract_, smartContractCString);

  const auto valueMapSS = detail::convertHashMap(env, value_);
// creata_  helper

/*
std::string add(const std::string &domain, const std::string &name,
                const txbuilder::Map &value,
                const std::string &smartContractName);
*/
  logger::debug(NameSpaceID + "::assetAdd") << "domainId: " << domainId
                                            << ", assetName: " << assetName;

  const auto ret = repository::asset::add(
      domain, name, value, smartContractName);

  return env->NewStringUTF(ret.c_str());
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_assetUpdate(
    JNIEnv *env, jclass, jstring uuid_, jobject) {
  const char *domainIdCString = env->GetStringUTFChars(domainId_, 0);
  const char *assetNameCString = env->GetStringUTFChars(assetName_, 0);
  const char *newValueCString = env->GetStringUTFChars(newValue_, 0);

  const auto domainId = std::string(domainIdCString);
  const auto assetName = std::string(assetNameCString);
  const auto newValue = std::string(newValueCString);

  env->ReleaseStringUTFChars(domainId_, domainIdCString);
  env->ReleaseStringUTFChars(assetName_, assetNameCString);
  env->ReleaseStringUTFChars(newValue_, newValueCString);

  return detail::JavaMakeBoolean(
      repository::asset::update(domainId, assetName, newValue));
}

JNIEXPORT jobject JNICALL
Java_test_repository_DomainRepository_assetRemove(JNIEnv *, jclass, jstring) {
  const char *domainIdCString = env->GetStringUTFChars(domainId_, 0);
  const char *assetNameCString = env->GetStringUTFChars(assetName_, 0);

  const auto domainId = std::string(domainIdCString);
  const auto assetName = std::string(assetNameCString);

  env->ReleaseStringUTFChars(domainId_, domainIdCString);
  env->ReleaseStringUTFChars(assetName_, assetNameCString);

  repository::asset::remove(domainId, assetName);
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_assetFindByUuid(
    JNIEnv *, jclass, jstring) {
  const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
  const auto uuid = std::string(uuidCString);

  env->ReleaseStringUTFChars(uuid_, uuidCString);

  logger::debug("domain repo jni :: assetFindByUuid") << "uuid: " << uuid;

  object::Asset asset = repository::asset::findByUuid(uuid);

  // These constant tags should be placed somewhere else.
  const auto DomainIdTag = "domainId";
  const auto AssetNameTag = "assetName";
  const auto ValueTag = "assetValue";

  std::unordered_map<std::string, std::string> params;
  {
    params[DomainIdTag] = asset.domain;
    params[AssetNameTag] = asset.name;
    params[ValueTag] = std::to_string(asset.value); // currently this is
                                                    // uint64_t. conversion from
                                                    // map to string?
  }

  logger::debug("domain repo :: asset jni")
      << "params[DomainIdTag]: " << params[DomainIdTag] << ", "
      << "params[AssetNameTag]: " << params[AssetNameTag] << ", "
      << "params[ValueTag]: " << params[ValueTag];

  return smart_contract::JavaMakeMap(env, params);
}