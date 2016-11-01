/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp

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

#ifndef CORE_DOMAIN_OBJECTS_MESSAGE_HPP_
#define CORE_DOMAIN_OBJECTS_MESSAGE_HPP_

namespace message {

class Message {
    std::string hash;
    std::string data;

public:
    Message(const std::string &data);

    virtual std::string getHash() const override;

    virtual std::string getRawData() const override;

    virtual std::string getAsText() const override;

    virtual unsigned long long int getTimestamp() const override;

    virtual abstract_transaction::TransactionType getType() const override;

    std::string getHash();
    std::string getRawData();
    std::string getAsText();
    std::string getAsJSON();
    abstract_transaction::TransactionType getType();
};

};  // namespace message

#endif  // CORE_DOMAIN_OBJECTS_MESSAGE_HPP_

