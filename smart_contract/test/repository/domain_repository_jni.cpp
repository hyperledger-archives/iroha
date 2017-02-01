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
#include "test_repository_DomainRepository.h"
#include "../../../core/repository/domain/account_repository.hpp"
#include "../../../core/repository/domain/asset_repository.hpp"
#include "../../../core/infra/smart_contract/jvm/java_virtual_machine.hpp"
#include "../../../core/util/logger.hpp"

#include <iostream>

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////////
// Account
////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_test_repository_DomainRepository_accountUpdateQuantity
  (JNIEnv *env, jclass cls, jstring uuid_, jstring assetName_, jlong newValue_)
{
    const char *uuidCString     = env->GetStringUTFChars(uuid_, 0);
    const char *assetNameCString = env->GetStringUTFChars(assetName_, 0);
    const auto newValue         = static_cast<int64_t>(newValue_);

    const auto uuid             = std::string(uuidCString);
    const auto assetName        = std::string(assetNameCString);

    env->ReleaseStringUTFChars(uuid_,       uuidCString);
    env->ReleaseStringUTFChars(assetName_,  assetNameCString);

    repository::account::update_quantity(uuid, assetName, newValue);
}

JNIEXPORT void JNICALL Java_test_repository_DomainRepository_accountAttach
  (JNIEnv *env, jclass cls, jstring uuid_, jstring assetName_, jlong assetDefault_)
{
    const char *uuidCString     = env->GetStringUTFChars(uuid_, 0);
    const char *assetNameCString = env->GetStringUTFChars(assetName_, 0);
    const auto assetDefault     = static_cast<int64_t>(assetDefault_);

    const auto uuid             = std::string(uuidCString);
    const auto assetName        = std::string(assetNameCString);

    env->ReleaseStringUTFChars(uuid_,       uuidCString);
    env->ReleaseStringUTFChars(assetName_,  assetNameCString);

    logger::debug("account repo jni") << uuid << ", " << assetName << ", " << assetDefault;

    repository::account::attach(uuid, assetName, assetDefault);
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_accountFindByUuid
  (JNIEnv *env, jclass cls, jstring uuid_)
{
    const char *uuidCString     = env->GetStringUTFChars(uuid_, 0);
    const auto uuid             = std::string(uuidCString);

    env->ReleaseStringUTFChars(uuid_, uuidCString);

    object::Account account = repository::account::findByUuid(uuid);
    
    // These should be placed somewhere else.
    const auto PublicKeyTag     = "publicKey";
    const auto AccountNameTag   = "accountName";

    std::unordered_map<std::string, std::string> params;
    {
        params[PublicKeyTag]    = account.publicKey;
        params[AccountNameTag]  = account.name;
    }

    logger::debug("account repository jni")
        << "params[PublicKeyTag]: "     << params[PublicKeyTag] << ", "
        << "params[AccountNameTag]: "   << params[AccountNameTag];
    // TODO: params["assets"]

    return smart_contract::JavaMakeMap(env, params);
}

JNIEXPORT jstring JNICALL Java_test_repository_DomainRepository_accountAdd
  (JNIEnv *env, jclass cls, jstring publicKey_, jstring alias_)
{
    const char *publicKeyCString    = env->GetStringUTFChars(publicKey_, 0);
    const char *aliasCString        = env->GetStringUTFChars(alias_, 0);

    const auto publicKey            = std::string(publicKeyCString);
    const auto alias                = std::string(aliasCString);

    env->ReleaseStringUTFChars(publicKey_,  publicKeyCString);
    env->ReleaseStringUTFChars(alias_,      aliasCString);

    std::cout << "Key " << publicKey << " alias:" << alias << std::endl;
    const auto ret = repository::account::add(publicKey, alias);

  return env->NewStringUTF(ret.c_str());
}

////////////////////////////////////////////////////////////////////////////////////
// Asset
////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT jstring JNICALL Java_test_repository_DomainRepository_assetAdd
    (JNIEnv *env, jclass cls, jstring domainId_, jstring assetName_, jstring value_)
{
    const char *domainIdCString     = env->GetStringUTFChars(domainId_, 0);
    const char *assetNameCString    = env->GetStringUTFChars(assetName_, 0);
    const char *valueCString        = env->GetStringUTFChars(value_, 0);

    const auto domainId     = std::string(domainIdCString);
    const auto assetName    = std::string(assetNameCString);
    const auto value        = std::string(valueCString);

    env->ReleaseStringUTFChars(domainId_,   domainIdCString);
    env->ReleaseStringUTFChars(assetName_,  assetNameCString);
    env->ReleaseStringUTFChars(value_,      valueCString);

    logger::debug("domain repo jni :: assetAdd") << "domainId: " << domainId << ", assetName: " << assetName;
    
    const auto ret = repository::asset::add(domainId, assetName, value); // TODO: asset::add(publicKey -> domainId, ,)
    return env->NewStringUTF(ret.c_str());
}

JNIEXPORT jobject JNICALL Java_test_repository_DomainRepository_assetFindByUuid
  (JNIEnv *env, jclass cls, jstring uuid_)
{
    const char *uuidCString = env->GetStringUTFChars(uuid_, 0);
    const auto uuid         = std::string(uuidCString);

    env->ReleaseStringUTFChars(uuid_, uuidCString);

    logger::debug("domain repo jni :: assetFindByUuid") << "uuid: " << uuid;

    object::Asset asset = repository::asset::findByUuid(uuid);

    // These constant tags should be placed somewhere else.
    const auto DomainIdTag      = "domainId";
    const auto AssetNameTag     = "assetName";
    const auto ValueTag         = "assetValue";

    std::unordered_map<std::string, std::string> params;
    {
        params[DomainIdTag]     = asset.domain;
        params[AssetNameTag]    = asset.name;
        params[ValueTag]        = std::to_string(asset.value); // currently this is uint64_t. conversion from map to string?
    }

    logger::debug("domain repo :: asset jni")
        << "params[DomainIdTag]: "  << params[DomainIdTag]  << ", "
        << "params[AssetNameTag]: " << params[AssetNameTag] << ", "
        << "params[ValueTag]: "     << params[ValueTag];

    return smart_contract::JavaMakeMap(env, params);
}

JNIEXPORT void JNICALL Java_test_repository_DomainRepository_assetUpdate
  (JNIEnv *env, jclass cls, jstring domainId_, jstring assetName_, jstring newValue_)
{
    const char *domainIdCString     = env->GetStringUTFChars(domainId_, 0);
    const char *assetNameCString    = env->GetStringUTFChars(assetName_, 0);
    const char *newValueCString     = env->GetStringUTFChars(newValue_,  0);

    const auto domainId     = std::string(domainIdCString);
    const auto assetName    = std::string(assetNameCString);
    const auto newValue     = std::string(newValueCString);

    env->ReleaseStringUTFChars(domainId_,  domainIdCString);
    env->ReleaseStringUTFChars(assetName_, assetNameCString);
    env->ReleaseStringUTFChars(newValue_,  newValueCString);

    repository::asset::update(domainId, assetName, newValue);
}

JNIEXPORT void JNICALL Java_test_repository_DomainRepository_assetRemove
  (JNIEnv *env, jclass, jstring domainId_, jstring assetName_)
{
    const char *domainIdCString     = env->GetStringUTFChars(domainId_, 0);
    const char *assetNameCString    = env->GetStringUTFChars(assetName_, 0);

    const auto domainId     = std::string(domainIdCString);
    const auto assetName    = std::string(assetNameCString);

    env->ReleaseStringUTFChars(domainId_,  domainIdCString);
    env->ReleaseStringUTFChars(assetName_, assetNameCString);

    repository::asset::remove(domainId, assetName);
}
