/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ametsuchi/impl/flat_file/flat_file.hpp"
#include <gtest/gtest.h>
#include "common/files.hpp"
#include "common/types.hpp"

namespace iroha {
  namespace ametsuchi {

    namespace block_store {

      class BlStore_Test : public ::testing::Test {
       protected:
        virtual void SetUp() {
          mkdir(block_store_path.c_str(),
                S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        virtual void TearDown() {
          iroha::remove_all(block_store_path);
        }
        std::string block_store_path = "/tmp/dump";
      };

      TEST_F(BlStore_Test, Read_Write_Test) {
        std::vector<uint8_t> block(100000, 5);
        auto bl_store = FlatFile::create(block_store_path);
        ASSERT_TRUE(bl_store);

        auto id = 1u;
        bl_store->add(id, block);
        auto id2 = 2u;
        bl_store->add(id2, block);

        auto res = bl_store->get(id);
        ASSERT_TRUE(res);
        ASSERT_FALSE(res->empty());
        ASSERT_EQ(*res, block);
      }

      TEST_F(BlStore_Test, InConsistency_Test) {
        // Adding blocks
        {
          std::vector<uint8_t> block(1000, 5);
          auto bl_store = FlatFile::create(block_store_path);
          ASSERT_TRUE(bl_store);
          // Adding three blocks
          auto id = 1u;
          bl_store->add(id, block);
          auto id2 = 2u;
          bl_store->add(id2, block);
          auto id3 = 3u;
          bl_store->add(id3, block);

          auto res = bl_store->get(id);
          ASSERT_TRUE(res);
          ASSERT_FALSE(res->empty());
          ASSERT_EQ(*res, block);
        }
        // Simulate removal of the block
        {
          // Remove file in the middle of the block store
          std::remove((block_store_path + "/0000000000000002").c_str());
          std::vector<uint8_t> block(1000, 5);
          auto bl_store = FlatFile::create(block_store_path);
          ASSERT_TRUE(bl_store);
          auto res = bl_store->last_id();
          // Must return 1
          ASSERT_EQ(res, 1);
        }
      }

    }  // namespace block_store

  }  // namespace ametsuchi
}  // namespace iroha
