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
#ifndef IROHA_STRING_WRAPPER_HPP
#define IROHA_STRING_WRAPPER_HPP

#include <string>
#include <vector>

namespace string_wrapper {

    class PublicKey {
    public:
        explicit PublicKey(std::string publicKey)
            : _publicKey(std::move(publicKey)) {}
        const auto& get()       const { return _publicKey; }
        const auto& operator*() const { return get();      }
    private:
        std::string _publicKey;
    };

    class DomainId {
    public:
        explicit DomainId(std::string domainId)
            : _domainId(std::move(domainId)) {}
        const auto& get()       const { return _domainId; }
        const auto& operator*() const { return get();     }
    private:
        std::string _domainId;
    };

    class AssetName {
    public:
        explicit AssetName(std::string assetName)
            : _assetName(std::move(assetName)) {}
        const auto& get()       const { return _assetName; }
        const auto& operator*() const { return get();      }
    private:
        std::string _assetName;
    };

    class AccountName {
    public:
        explicit AccountName(std::string accountName)
            : _accountName(std::move(accountName)) {}
        const auto& get()       const { return _accountName; }
        const auto& operator*() const { return get();        }
    private:
        std::string _accountName;
    };

    class Alias {
    public:
        explicit Alias(std::string alias)
            : _alias(std::move(alias)) {}
        const auto& get()       const { return _alias; }
        const auto& operator*() const { return get();  }        
    private:
        std::string _alias;
    };

}

#endif //IROHA_STRING_WRAPPER_HPP
