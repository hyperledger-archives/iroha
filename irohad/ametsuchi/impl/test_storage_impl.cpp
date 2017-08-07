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

#include <utility>
#include "ametsuchi/impl/test_storage_impl.hpp"
#include "common/files.hpp"

namespace iroha {
  namespace ametsuchi {

    TestStorageImpl::TestStorageImpl(std::string &block_store_dir,
                                     std::string &redis_host,
                                     size_t redis_port,
                                     std::string &postgres_options,
                                     std::unique_ptr<FlatFile> &block_store,
                                     std::unique_ptr<cpp_redis::redis_client> &index,
                                     std::unique_ptr<pqxx::lazyconnection> &wsv_connection,
                                     std::unique_ptr<pqxx::nontransaction> &wsv_transaction,
                                     std::unique_ptr<WsvQuery> &wsv)
        : StorageImpl(block_store_dir,
                      redis_host,
                      redis_port,
                      postgres_options,
                      std::move(block_store),
                      std::move(index),
                      std::move(wsv_connection),
                      std::move(wsv_transaction),
                      std::move(wsv)) {
      log_ = logger::log("TestStorage");
    }

    void TestStorageImpl::insertBlock(model::Block block) {
      log_->warn("insertBlock not implemented");
    }

    void TestStorageImpl::dropStorage() {
      log_->info("Drop ledger");
      auto drop =
          "DROP TABLE IF EXISTS account_has_asset;\n"
              "DROP TABLE IF EXISTS account_has_signatory;\n"
              "DROP TABLE IF EXISTS peer;\n"
              "DROP TABLE IF EXISTS account;\n"
              "DROP TABLE IF EXISTS exchange;\n"
              "DROP TABLE IF EXISTS asset;\n"
              "DROP TABLE IF EXISTS domain;\n"
              "DROP TABLE IF EXISTS signatory;";

      // erase db
      wsv_transaction_->exec(drop);
      wsv_transaction_->commit();

      // erase tx index
      index_->flushall();
      index_->sync_commit();

      // erase blocks
      remove_all(block_store_dir_);
    }

  }  // namespace ametsuchi
}  // namespace iroha