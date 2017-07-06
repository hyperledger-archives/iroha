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

#include <ametsuchi/impl/mutable_storage_impl.hpp>
#include <ametsuchi/impl/storage_impl.hpp>
#include <ametsuchi/impl/temporary_wsv_impl.hpp>

namespace iroha {
  namespace ametsuchi {

    std::unique_ptr<TemporaryWsv> StorageImpl::createTemporaryWsv() {
      return std::make_unique<TemporaryWsvImpl>();
    }

    std::unique_ptr<MutableStorage> StorageImpl::createMutableStorage() {
      return std::make_unique<MutableStorageImpl>();
    }

    std::unique_ptr<StorageImpl> StorageImpl::create(
        std::string block_store_dir, std::string redis_host,
        std::size_t redis_port, std::string postgres_options) {
      auto block_store = FlatFile::create(block_store_dir);
      if (!block_store) {
        // TODO log error
        return nullptr;
      }

      auto index = std::make_unique<cpp_redis::redis_client>();
      try {
        index->connect(redis_host, redis_port);
      } catch (const cpp_redis::redis_error &e) {
        // TODO log error
        return nullptr;
      }

      auto postgres_connection =
          std::make_unique<pqxx::lazyconnection>(postgres_options);
      try {
        postgres_connection->activate();
      } catch (const pqxx::broken_connection &e) {
        // TODO log error
        return nullptr;
      }
      auto transaction = std::make_shared<pqxx::nontransaction>(
          postgres_connection, "storage");

      return std::unique_ptr<StorageImpl>(new StorageImpl());
    }

    StorageImpl::StorageImpl(std::unique_ptr<FlatFile> block_store,
                             std::unique_ptr<cpp_redis::redis_client> index)
        : block_store_(std::move(block_store)), index_(std::move(index)) {
      transaction_->exec();
    }

    void StorageImpl::commit(std::unique_ptr<MutableStorage> mutableStorage) {
      auto storage = std::move(mutableStorage);  // get ownership of storage
      std::unique_lock<std::shared_timed_mutex> write(rw_lock_);
    }

  }  // namespace ametsuchi
}  // namespace iroha