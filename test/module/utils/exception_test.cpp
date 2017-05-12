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
#include <gtest/gtest.h>
#include <utils/exception.hpp>

TEST(exception, None) {
  ASSERT_THROW(
    throw exception::None(),
    exception::None);
  ASSERT_THROW(
    throw exception::None(),
    exception::NoError);

  try {
    throw exception::None();
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("NO_ERROR") != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("NO_ERROR") != std::string::npos);
  }
}

TEST(exception, NotImplementedException) {
  std::string functionName("test-function");
  std::string filename("test-file");
  ASSERT_THROW(
    throw exception::NotImplementedException(functionName, filename),
    exception::NotImplementedException);
  ASSERT_THROW(
    throw exception::NotImplementedException(functionName, filename),
    exception::Ordinary);

  try {
    throw exception::NotImplementedException(functionName, filename);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info1.find(functionName) != std::string::npos);
    ASSERT_TRUE(info1.find(filename) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info2.find(functionName) != std::string::npos);
    ASSERT_TRUE(info2.find(filename) != std::string::npos);
  }
}

TEST(exception, InvalidCastException1) {
  std::string from("test-from");
  std::string to("test-to");
  std::string filename("test-filename");
  ASSERT_THROW(
    throw exception::InvalidCastException(from, to, filename),
    exception::InvalidCastException);
  ASSERT_THROW(
    throw exception::InvalidCastException(from, to, filename),
    exception::Ordinary);

  try {
    throw exception::InvalidCastException(from, to, filename);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info1.find(from) != std::string::npos);
    ASSERT_TRUE(info1.find(to) != std::string::npos);
    ASSERT_TRUE(info1.find(filename) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info2.find(from) != std::string::npos);
    ASSERT_TRUE(info2.find(to) != std::string::npos);
    ASSERT_TRUE(info2.find(filename) != std::string::npos);
  }
}

TEST(exception, InvalidCastException2) {
  std::string meg("test-message");
  std::string filename("test-file");
  ASSERT_THROW(
    throw exception::InvalidCastException(meg, filename),
    exception::InvalidCastException);
  ASSERT_THROW(
    throw exception::InvalidCastException(meg, filename),
    exception::Ordinary);

  try {
    throw exception::InvalidCastException(meg, filename);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info1.find(meg) != std::string::npos);
    ASSERT_TRUE(info1.find(filename) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info2.find(meg) != std::string::npos);
    ASSERT_TRUE(info2.find(filename) != std::string::npos);
  }
}

TEST(exception, DuplicateSetArgumentException) {
  std::string buildTarget("test-buildTarget");
  std::string duplicateMember("test-duplicateMember");
  ASSERT_THROW(
    throw exception::DuplicateSetArgumentException(buildTarget, duplicateMember),
    exception::DuplicateSetArgumentException);
  ASSERT_THROW(
    throw exception::DuplicateSetArgumentException(buildTarget, duplicateMember),
    exception::Ordinary);

  try {
    throw exception::DuplicateSetArgumentException(buildTarget, duplicateMember);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info1.find(buildTarget) != std::string::npos);
    ASSERT_TRUE(info1.find(duplicateMember) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info2.find(buildTarget) != std::string::npos);
    ASSERT_TRUE(info2.find(duplicateMember) != std::string::npos);
  }
}

TEST(exception, UnsetBuildArgumentsException) {
  std::string buildTarget("test-buildTarget");
  std::string unsetMembers("test-unsetMembers");
  ASSERT_THROW(
    throw exception::UnsetBuildArgumentsException(buildTarget, unsetMembers),
    exception::UnsetBuildArgumentsException);
  ASSERT_THROW(
    throw exception::UnsetBuildArgumentsException(buildTarget, unsetMembers),
    exception::Ordinary);

  try {
    throw exception::UnsetBuildArgumentsException(buildTarget, unsetMembers);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info1.find(buildTarget) != std::string::npos);
    ASSERT_TRUE(info1.find(unsetMembers) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info2.find(buildTarget) != std::string::npos);
    ASSERT_TRUE(info2.find(unsetMembers) != std::string::npos);
  }
}

TEST(exception, NotFoundPathException) {
  std::string path("test-path");
  ASSERT_THROW(
    throw exception::NotFoundPathException(path),
    exception::NotFoundPathException);
  ASSERT_THROW(
    throw exception::NotFoundPathException(path),
    exception::Insecure);

  try {
    throw exception::NotFoundPathException(path);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(path) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(path) != std::string::npos);
  }
}

TEST(exception, RequirePropertyMissingException) {
  std::string buildTarget("test-buildTarget");
  std::string message("test-message");
  ASSERT_THROW(
    throw exception::RequirePropertyMissingException(buildTarget, message),
    exception::RequirePropertyMissingException);
  ASSERT_THROW(
    throw exception::RequirePropertyMissingException(buildTarget, message),
    exception::IrohaException);

  try {
    throw exception::RequirePropertyMissingException(buildTarget, message);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find(buildTarget) != std::string::npos);
    ASSERT_TRUE(info1.find(message) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find(buildTarget) != std::string::npos);
    ASSERT_TRUE(info2.find(message) != std::string::npos);
  }
}

TEST(exception, ConfigException) {
  std::string message("test-message");
  std::string funcname("test-function-name");
  ASSERT_THROW(
    throw exception::config::ConfigException(message, funcname),
    exception::config::ConfigException);
  ASSERT_THROW(
    throw exception::config::ConfigException(message, funcname),
    exception::Insecure);

  try {
    throw exception::config::ConfigException(message, funcname);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(message) != std::string::npos);
    ASSERT_TRUE(info1.find(funcname) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(message) != std::string::npos);
    ASSERT_TRUE(info2.find(funcname) != std::string::npos);
  }
}

TEST(exception, ParseException) {
  std::string target("test-target");
  ASSERT_THROW(
    throw exception::config::ParseException(target, true),
    exception::config::ParseException);
  ASSERT_THROW(
    throw exception::config::ParseException(target, true),
    exception::Insecure);

  try {
    throw exception::config::ParseException(target, true);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(target) != std::string::npos);
    ASSERT_TRUE(info1.find("default") != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(target) != std::string::npos);
    ASSERT_TRUE(info2.find("default") != std::string::npos);
  }
  try {
    throw exception::config::ParseException(target, false);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(target) != std::string::npos);
    ASSERT_TRUE(info1.find("default") == std::string::npos);
  }
}

TEST(exception, UndefinedIrohaHomeException) {
  ASSERT_THROW(
    throw exception::config::UndefinedIrohaHomeException(),
    exception::config::UndefinedIrohaHomeException);
  ASSERT_THROW(
    throw exception::config::UndefinedIrohaHomeException(),
    exception::Critical);

  try {
    throw exception::config::UndefinedIrohaHomeException();
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("CRITICAL") != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("CRITICAL") != std::string::npos);
  }
}

TEST(exception, NullptrException) {
  std::string target("test-target");
  ASSERT_THROW(
    throw exception::connection::NullptrException(target),
    exception::connection::NullptrException);
  ASSERT_THROW(
    throw exception::connection::NullptrException(target),
    exception::Ordinary);

  try {
    throw exception::connection::NullptrException(target);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info1.find(target) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info2.find(target) != std::string::npos);
  }
}

TEST(exception, FailedToCreateConsensusEvent) {
  ASSERT_THROW(
    throw exception::connection::FailedToCreateConsensusEvent(),
    exception::connection::FailedToCreateConsensusEvent);
  ASSERT_THROW(
    throw exception::connection::FailedToCreateConsensusEvent(),
    exception::Ordinary);

  try {
    throw exception::connection::FailedToCreateConsensusEvent();
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
  }
}

TEST(exception, RPCConnectionException) {
  int code(999);
  std::string message("test-message");
  ASSERT_THROW(
    throw exception::connection::RPCConnectionException(code, message),
    exception::connection::RPCConnectionException);
  ASSERT_THROW(
    throw exception::connection::RPCConnectionException(code, message),
    exception::Ordinary);

  try {
    throw exception::connection::RPCConnectionException(code, message);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info1.find(std::to_string(code)) != std::string::npos);
    ASSERT_TRUE(info1.find(message) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
    ASSERT_TRUE(info2.find(std::to_string(code)) != std::string::npos);
    ASSERT_TRUE(info2.find(message) != std::string::npos);
  }
}

TEST(exception, InvalidTransactionException) {
  ASSERT_THROW(
    throw exception::connection::InvalidTransactionException(),
    exception::connection::InvalidTransactionException);
  ASSERT_THROW(
    throw exception::connection::InvalidTransactionException(),
    exception::Ordinary);

  try {
    throw exception::connection::InvalidTransactionException();
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("ERROR") != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("ERROR") != std::string::npos);
  }
}

TEST(exception, DuplicationIPException) {
  std::string ip("test-ip");
  ASSERT_THROW(
    throw exception::service::DuplicationIPException(ip),
    exception::service::DuplicationIPException);
  ASSERT_THROW(
    throw exception::service::DuplicationIPException(ip),
    exception::Insecure);

  try {
    throw exception::service::DuplicationIPException(ip);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(ip) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(ip) != std::string::npos);
  }
}

TEST(exception, DuplicationPublicKeyException) {
  std::string publicKey("test-publicKey");
  ASSERT_THROW(
    throw exception::service::DuplicationPublicKeyException(publicKey),
    exception::service::DuplicationPublicKeyException);
  ASSERT_THROW(
    throw exception::service::DuplicationPublicKeyException(publicKey),
    exception::Insecure);

  try {
    throw exception::service::DuplicationPublicKeyException(publicKey);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(publicKey) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(publicKey) != std::string::npos);
  }
}

TEST(exception, UnExistFindPeerException) {
  std::string publicKey("test-publicKey");
  ASSERT_THROW(
    throw exception::service::UnExistFindPeerException(publicKey),
    exception::service::UnExistFindPeerException);
  ASSERT_THROW(
    throw exception::service::UnExistFindPeerException(publicKey),
    exception::Insecure);

  try {
    throw exception::service::UnExistFindPeerException(publicKey);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(publicKey) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(publicKey) != std::string::npos);
  }
}

TEST(exception, InvalidKeyException) {
  std::string message("test-message");
  ASSERT_THROW(
    throw exception::crypto::InvalidKeyException(message),
    exception::crypto::InvalidKeyException);
  ASSERT_THROW(
    throw exception::crypto::InvalidKeyException(message),
    exception::Insecure);

  try {
    throw exception::crypto::InvalidKeyException(message);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(message) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(message) != std::string::npos);
  }
}

TEST(exception, InvalidMessageLengthException) {
  std::string message("test-message");
  ASSERT_THROW(
    throw exception::crypto::InvalidMessageLengthException(message),
    exception::crypto::InvalidMessageLengthException);
  ASSERT_THROW(
    throw exception::crypto::InvalidMessageLengthException(message),
    exception::Insecure);

  try {
    throw exception::crypto::InvalidMessageLengthException(message);
  }
  catch (exception::IrohaException &e) {
    std::string info1(e.what());
    ASSERT_TRUE(info1.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info1.find(message) != std::string::npos);
    std::string info2(e.message());
    ASSERT_TRUE(info2.find("INSECURE") != std::string::npos);
    ASSERT_TRUE(info2.find(message) != std::string::npos);
  }
}

