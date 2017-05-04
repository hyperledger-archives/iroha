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
#include <infra/config/config_utils.hpp>

TEST(ConfigUtils, appendSlashIfNeeded) {
  using config::detail::append_slash_if_needed;
  ASSERT_STREQ(append_slash_if_needed("").c_str(), "/");
  ASSERT_STREQ(append_slash_if_needed("////").c_str(), "////");
  ASSERT_STREQ(append_slash_if_needed("/hoge").c_str(), "/hoge/");
  ASSERT_STREQ(append_slash_if_needed("foo").c_str(), "foo/");
  ASSERT_STREQ(append_slash_if_needed("/opt/iroha").c_str(), "/opt/iroha/");
  ASSERT_STREQ(append_slash_if_needed("/opt/iroha/").c_str(), "/opt/iroha/");
}
