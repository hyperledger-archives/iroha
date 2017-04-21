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

#include <stdexcept>

#include "exception.hpp"

namespace exception {

IrohaException::IrohaException(const std::string &message)
    : message_(message) {}

IrohaException::~IrohaException() {}

const char *IrohaException::what() const throw() { return message_.c_str(); }

NotImplementedException::NotImplementedException(
    const std::string &functionName, const std::string &filename)
    : IrohaException(
          "TODO: Sorry, function [" + functionName + "] in file " + filename +
          " is not yet implemented, would you like to contribute it?") {}

ParseFromStringException::ParseFromStringException(const std::string &filename)
    : IrohaException("ParseFromStringException in file " + filename) {}

InvalidCastException::InvalidCastException(const std::string &from,
                                           const std::string &to,
                                           const std::string &filename)
    : IrohaException("InvalidCastException in file " + filename +
                     ". Cannot cast from " + from + " to " + to) {}

InvalidCastException::InvalidCastException(const std::string &meg,
                                           const std::string &filename)
    : IrohaException("InvalidCastException in " + filename + ". " + meg) {}

namespace config {
ConfigException::ConfigException(const std::string &message)
    : IrohaException("ConfigException: " + message) {}
}  // namespace config

namespace service {
DuplicationIPException::DuplicationIPException(const std::string &ip)
    : IrohaException("DuplicationIPException : IP = " + ip) {}

DuplicationPublicKeyException::DuplicationPublicKeyException(
    const std::string &publicKey)
    : IrohaException("DuplicationPublicKeyException : publicKey = " +
                     publicKey) {}

UnExistFindPeerException::UnExistFindPeerException(const std::string &publicKey)
    : IrohaException("UnExistFindPeerException : publicKey = " + publicKey) {}
}  // namespace service

namespace crypto {
InvalidKeyException::InvalidKeyException(const std::string &message)
    : IrohaException("Keyfile is invalid, cause is: " + message) {}
InvalidMessageLengthException::InvalidMessageLengthException(
    const std::string &message)
    : IrohaException("Message " + message + " has wrong length") {}
}  // namespace crypto

namespace ordinary {
DuplicateSetArgumentException::DuplicateSetArgumentException(
    const std::string &buildTarget, const std::string &duplicateMember)
    : IrohaException("DuplicateSetArgumentException in " + buildTarget +
                     ", argument: " + duplicateMember) {}

UnsetBuildArgumentsException::UnsetBuildArgumentsException(
    const std::string &buildTarget, const std::string &unsetMembers)
    : IrohaException("UnsetBuildArgumentsException in " + buildTarget +
                     ", arguments: " + unsetMembers) {}
}  // namespace ordinary
}  // namespace exception
