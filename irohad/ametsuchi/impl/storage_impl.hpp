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

#ifndef IROHA_STORAGE_IMPL_HPP
#define IROHA_STORAGE_IMPL_HPP

#include <ametsuchi/impl/flat_file/flat_file.hpp>
#include <ametsuchi/storage.hpp>
#include <cpp_redis/cpp_redis>
#include <nonstd/optional.hpp>
#include <pqxx/pqxx>
#include <shared_mutex>

namespace iroha {
  namespace ametsuchi {
    class StorageImpl : public Storage {
     public:
      static std::unique_ptr<StorageImpl> create(
          std::string block_store_dir, std::string redis_host,
          std::size_t redis_port, std::string postgres_connection);
      std::unique_ptr<TemporaryWsv> createTemporaryWsv() override;
      std::unique_ptr<MutableStorage> createMutableStorage() override;
      void commit(std::unique_ptr<MutableStorage> mutableStorage) override;

     private:
      StorageImpl(std::string block_store_dir, std::string redis_host,
                  std::size_t redis_port, std::string postgres_options,
                  std::unique_ptr<FlatFile> block_store,
                  std::unique_ptr<cpp_redis::redis_client> index,
                  std::unique_ptr<pqxx::nontransaction> wsv_transaction,
                  std::unique_ptr<WsvQuery> wsv);
      // Storage info
      const std::string block_store_dir_;
      const std::string redis_host_;
      const std::size_t redis_port_;
      const std::string postgres_options_;

      std::unique_ptr<FlatFile> block_store_;
      std::unique_ptr<cpp_redis::redis_client> index_;

      std::unique_ptr<pqxx::nontransaction> wsv_transaction_;
      std::unique_ptr<WsvQuery> wsv_;

      // Allows multiple readers and a single writer
      std::shared_timed_mutex rw_lock_;
    };
  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_STORAGE_IMPL_HPP
