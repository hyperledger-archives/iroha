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
#include "../repository_AccountRepository.h"
#include "../../../core/repository/domain/account_repository.hpp"
#include "../../../core/infra/smart_contract/jvm/java_virtual_machine.hpp"
#include "../../../core/util/logger.hpp"

#include <iostream>

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <assert.h>

JNIEXPORT void JNICALL Java_repository_AccountRepository_updateQuantity
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

JNIEXPORT void JNICALL Java_repository_AccountRepository_attach
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

JNIEXPORT jobject JNICALL Java_repository_AccountRepository_findByUuid
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

JNIEXPORT jstring JNICALL Java_repository_AccountRepository_add
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
