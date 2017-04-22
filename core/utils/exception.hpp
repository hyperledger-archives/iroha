/*
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *          http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_UTILS_EXCEPTION_INSTANCES_HPP_
#define IROHA_UTILS_EXCEPTION_INSTANCES_HPP_

#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace exception {

class IrohaException : public std::exception {
 public:
  explicit IrohaException(const std::string &);

  virtual ~IrohaException();
  virtual const char *what() const noexcept;

 protected:
  std::string message_;
};

class None : public IrohaException {
 public:
  None();
};

class NotImplementedException : public IrohaException {
 public:
  explicit NotImplementedException(const std::string &functionName,
                                   const std::string &filename);
};

class ParseFromStringException : public IrohaException {
 public:
  explicit ParseFromStringException(const std::string &filename);
};

class InvalidCastException : public IrohaException {
 public:
  InvalidCastException(const std::string &from, const std::string &to,
                       const std::string &filename);
  InvalidCastException(const std::string &meg, const std::string &filename);
};

class DuplicateSetArgumentException : public IrohaException {
 public:
  DuplicateSetArgumentException(const std::string &, const std::string &);
};
class UnsetBuildArgumentsException : public IrohaException {
 public:
  UnsetBuildArgumentsException(const std::string &, const std::string &);
};

namespace config {

class ConfigException : public IrohaException {
 public:
  ConfigException(const std::string &message, const std::string& filename);
};

}  // namespace config

namespace service {

class DuplicationIPException : public IrohaException {
 public:
  explicit DuplicationIPException(const std::string &);
};

class DuplicationPublicKeyException : public IrohaException {
 public:
  explicit DuplicationPublicKeyException(const std::string &);
};

class UnExistFindPeerException : public IrohaException {
 public:
  explicit UnExistFindPeerException(const std::string &);
};

}  // namespace service

namespace crypto {

class InvalidKeyException : public IrohaException {
 public:
  explicit InvalidKeyException(const std::string &);
};

class InvalidMessageLengthException : public IrohaException {
 public:
  explicit InvalidMessageLengthException(const std::string &);
};

}  // namespace crypto
}  // namespace exception

#endif  // IROHA_UTILS_EXCEPTION_INSTANCES_HPP_
