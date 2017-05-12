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
#include <utils/logger.hpp>

TEST(logger, debug) {
  logger::setLogLevel(logger::LogLevel::Debug);
  testing::internal::CaptureStdout();
  logger::debug("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap.find("DEBUG") != std::string::npos);
  ASSERT_TRUE(cap.find("[test-module]") != std::string::npos);
  ASSERT_TRUE(cap.find("message1") != std::string::npos);
}

TEST(logger, explore) {
  logger::setLogLevel(logger::LogLevel::Debug);
  testing::internal::CaptureStdout();
  logger::explore("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  //ASSERT_TRUE(cap.find("EXPLORE") != std::string::npos);
  ASSERT_TRUE(cap.find("[test-module]") != std::string::npos);
  ASSERT_TRUE(cap.find("message1") != std::string::npos);
}

TEST(logger, info) {
  logger::setLogLevel(logger::LogLevel::Debug);
  testing::internal::CaptureStdout();
  logger::info("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap.find("INFO") != std::string::npos);
  ASSERT_TRUE(cap.find("[test-module]") != std::string::npos);
  ASSERT_TRUE(cap.find("message1") != std::string::npos);
}

TEST(logger, warning) {
  logger::setLogLevel(logger::LogLevel::Debug);
  testing::internal::CaptureStdout();
  logger::warning("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap.find("WARNING") != std::string::npos);
  ASSERT_TRUE(cap.find("[test-module]") != std::string::npos);
  ASSERT_TRUE(cap.find("message1") != std::string::npos);
}

TEST(logger, error) {
  logger::setLogLevel(logger::LogLevel::Debug);
  testing::internal::CaptureStderr();
  logger::error("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(cap.find("ERROR") != std::string::npos);
  ASSERT_TRUE(cap.find("[test-module]") != std::string::npos);
  ASSERT_TRUE(cap.find("message1") != std::string::npos);
}

TEST(logger, fatal) {
  logger::setLogLevel(logger::LogLevel::Debug);
  testing::internal::CaptureStderr();
  logger::fatal("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(cap.find("FATAL") != std::string::npos);
  ASSERT_TRUE(cap.find("[test-module]") != std::string::npos);
  ASSERT_TRUE(cap.find("message1") != std::string::npos);
}

TEST(logger, setLogLevel1) {
  logger::setLogLevel(logger::LogLevel::Explore);
  testing::internal::CaptureStdout();
  logger::debug("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap == std::string(""));
}

TEST(logger, setLogLevel2) {
  logger::setLogLevel(logger::LogLevel::Info);
  testing::internal::CaptureStdout();
  logger::explore("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap == std::string(""));
}

TEST(logger, setLogLevel3) {
  logger::setLogLevel(logger::LogLevel::Warning);
  testing::internal::CaptureStdout();
  logger::info("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap == std::string(""));
}

TEST(logger, setLogLevel4) {
  logger::setLogLevel(logger::LogLevel::Error);
  testing::internal::CaptureStdout();
  logger::warning("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap == std::string(""));
}

TEST(logger, setLogLevel5) {
  logger::setLogLevel(logger::LogLevel::Fatal);
  testing::internal::CaptureStderr();
  logger::error("test-module") << "message1";
  std::string cap = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(cap == std::string(""));
}

TEST(logger, manyMessages) {
  logger::setLogLevel(logger::LogLevel::Debug);
  testing::internal::CaptureStdout();
  logger::debug("test-module")
    << "message1" << " message2" << " message3" << " message4" << " message5";
  std::string cap = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(cap.find("DEBUG") != std::string::npos);
  ASSERT_TRUE(cap.find("[test-module]") != std::string::npos);
  ASSERT_TRUE(cap.find("message1 message2 message3 message4 message5") != std::string::npos);
}
