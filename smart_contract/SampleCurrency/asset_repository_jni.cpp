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
#include "repository_AssetRepository.h"
#include "../../core/repository/domain/asset_repository.hpp"
#include "../../core/util/serialized_type_switcher.hpp"
#include "../../core/util/logger.hpp"
#include <string>
#include <memory>
#include <vector>
#include <assert.h>

namespace detail {
    inline std::string getTypeID(std::string const& s) {
        assert(s.size() >= util::type_switcher::TypeIDSize);
        return s.substr(0, util::type_switcher::TypeIDSize);
    }
    inline std::string getValueStr(std::string const& s) {
        assert(s.size() >= util::type_switcher::TypeIDSize);
        return s.substr(util::type_switcher::TypeIDSize);
    }
}

JNIEXPORT void JNICALL Java_repository_AssetRepository_add
    (JNIEnv *env, jclass cls, jstring publicKey_, jstring assetName_, jstring value_) {
    const char *publicKeyCString    = env->GetStringUTFChars(publicKey_, 0);
    const char *assetNameCString    = env->GetStringUTFChars(assetName_, 0);
    const char *valueCString        = env->GetStringUTFChars(value_, 0);

    const auto publicKey    = std::string(publicKeyCString);
    const auto assetName    = std::string(assetNameCString);
    const auto value        = std::string(valueCString);  // WIP: valueは現状はシリアライズされた型で分析する必要がある

    env->ReleaseStringUTFChars(publicKey_, publicKeyCString);
    env->ReleaseStringUTFChars(assetName_, assetNameCString);
    env->ReleaseStringUTFChars(value_, valueCString);

    repository::asset::add(publicKey, assetName, value);

}

//JNIEXPORT std::string JNICALL Java_AssetRepository_findOne(std::string key) {}
//JNIEXPORT void JNICALL Java_Repository_update(std::string key, std::string value) {}
//JNIEXPORT void JNICALL Java_Repository_remove(std::string key) {}

//std::vector<std::unique_ptr<T> > findAll(std::string key);
//std::unique_ptr<T> findOrElse(std::string key, T defaultVale);
//bool isExist(std::string key);
