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

#include "exception_tag.hpp"

namespace exception {

class IrohaException : public std::exception,
                       public exception_tag::ExceptionTag {
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

class NotImplementedException : public IrohaException,
                                public exception_tag::HelpWanted {
 public:
  explicit NotImplementedException(const std::string &functionName,
                                   const std::string &filename);
};

class InvalidCastException : public IrohaException,
                             public exception_tag::Critical {
 public:
  InvalidCastException(const std::string &from, const std::string &to,
                       const std::string &filename);
  InvalidCastException(const std::string &meg, const std::string &filename);
};

class DuplicateSetArgumentException : public IrohaException,
                                      public exception_tag::Critical {
 public:
  DuplicateSetArgumentException(const std::string &, const std::string &);
};

class UnsetBuildArgumentsException : public IrohaException,
                                     public exception_tag::Critical {
 public:
  UnsetBuildArgumentsException(const std::string &, const std::string &);
};

class NotFoundPathException : public IrohaException,
                              public exception_tag::ConfigError {
 public:
  NotFoundPathException(const std::string &path);
};

namespace config {

// deprecated, will remove
class ConfigException : public IrohaException,
                        public exception_tag::ConfigError {
 public:
  ConfigException(const std::string &message, const std::string &filename);
};

class ParseException : public IrohaException,
                       public exception_tag::ConfigError {
 public:
  ParseException(const std::string &target, bool setDefaultMessage = false);
};

class UndefinedIrohaHomeException : public IrohaException,
                                    public exception_tag::ConfigError {
 public:
  UndefinedIrohaHomeException();
};

}  // namespace config

namespace connection {
class NullptrException : public IrohaException,
                         public exception_tag::Critical {
 public:
  NullptrException(const std::string &target);
};

class FailedToCreateConsensusEvent : public IrohaException,
                                     public exception_tag::Critical {
 public:
  FailedToCreateConsensusEvent();
};
}  // namespace connection

namespace service {

class DuplicationIPException : public IrohaException,
                               public exception_tag::ConfigError {
 public:
  explicit DuplicationIPException(const std::string &);
};

class DuplicationPublicKeyException : public IrohaException,
                                      public exception_tag::ConfigError {
 public:
  explicit DuplicationPublicKeyException(const std::string &);
};

class UnExistFindPeerException : public IrohaException,
                                 public exception_tag::Critical {
 public:
  explicit UnExistFindPeerException(const std::string &);
};

}  // namespace service

namespace crypto {

class InvalidKeyException : public IrohaException,
                            public exception_tag::ConfigError {
 public:
  explicit InvalidKeyException(const std::string &);
};

class InvalidMessageLengthException : public IrohaException,
                                      public exception_tag::Critical {
 public:
  explicit InvalidMessageLengthException(const std::string &);
};

}  // namespace crypto
}  // namespace exception

#endif  // IROHA_UTILS_EXCEPTION_INSTANCES_HPP_
