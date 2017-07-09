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
#include <ametsuchi/impl/postgres_command_executor.hpp>
#include <ametsuchi/impl/postgres_wsv_query.hpp>
#include <ametsuchi/impl/storage_impl.hpp>
#include <ametsuchi/impl/temporary_wsv_impl.hpp>

namespace iroha {
  namespace ametsuchi {

    std::unique_ptr<TemporaryWsv> StorageImpl::createTemporaryWsv() {
      // TODO lock

      auto postgres_connection =
          std::make_unique<pqxx::lazyconnection>(postgres_options_);
      try {
        postgres_connection->activate();
      } catch (const pqxx::broken_connection &e) {
        // TODO log error
        return nullptr;
      }
      auto wsv_transaction = std::make_unique<pqxx::nontransaction>(
          *postgres_connection, "TemporaryWsv");
      std::unique_ptr<WsvQuery> wsv =
          std::make_unique<PostgresWsvQuery>(wsv_transaction);
      std::unique_ptr<CommandExecutor> executor =
          std::make_unique<PostgresCommandExecutor>(wsv_transaction);

      return std::make_unique<TemporaryWsvImpl>(
          std::move(wsv_transaction), std::move(wsv), std::move(executor));
    }

    std::unique_ptr<MutableStorage> StorageImpl::createMutableStorage() {
      // TODO lock

      auto postgres_connection =
          std::make_unique<pqxx::lazyconnection>(postgres_options_);
      try {
        postgres_connection->activate();
      } catch (const pqxx::broken_connection &e) {
        // TODO log error
        return nullptr;
      }
      auto wsv_transaction = std::make_unique<pqxx::nontransaction>(
          *postgres_connection, "TemporaryWsv");
      std::unique_ptr<WsvQuery> wsv =
          std::make_unique<PostgresWsvQuery>(wsv_transaction);
      std::unique_ptr<CommandExecutor> executor =
          std::make_unique<PostgresCommandExecutor>(wsv_transaction);

      auto index = std::make_unique<cpp_redis::redis_client>();
      try {
        index->connect(redis_host_, redis_port_);
      } catch (const cpp_redis::redis_error &e) {
        // TODO log error
        return nullptr;
      }

      return std::make_unique<MutableStorageImpl>(
          block_store_, std::move(index), std::move(wsv_transaction),
          std::move(wsv), std::move(executor));
    }

    std::unique_ptr<StorageImpl> StorageImpl::create(
        std::string block_store_dir, std::string redis_host,
        std::size_t redis_port, std::string postgres_options) {
      // TODO lock

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
      auto wsv_transaction = std::make_unique<pqxx::nontransaction>(
          *postgres_connection, "Storage");
      std::unique_ptr<WsvQuery> wsv =
          std::make_unique<PostgresWsvQuery>(wsv_transaction);

      return std::unique_ptr<StorageImpl>(new StorageImpl(
          block_store_dir, redis_host, redis_port, postgres_options,
          std::move(block_store), std::move(index), std::move(wsv_transaction),
          std::move(wsv)));
    }

    void StorageImpl::commit(std::unique_ptr<MutableStorage> mutableStorage) {
      auto storage = std::move(mutableStorage);  // get ownership of storage
      std::unique_lock<std::shared_timed_mutex> write(rw_lock_);
    }

    StorageImpl::StorageImpl(
        std::string block_store_dir, std::string redis_host,
        std::size_t redis_port, std::string postgres_options,
        std::unique_ptr<FlatFile> block_store,
        std::unique_ptr<cpp_redis::redis_client> index,
        std::unique_ptr<pqxx::nontransaction> wsv_transaction,
        std::unique_ptr<WsvQuery> wsv)
        : block_store_dir_(block_store_dir),
          redis_host_(redis_host),
          redis_port_(redis_port),
          postgres_options_(postgres_options),
          block_store_(std::move(block_store)),
          index_(std::move(index)),
          wsv_transaction_(std::move(wsv_transaction)),
          wsv_(std::move(wsv)) {
      wsv_transaction_->exec(
          "SET SESSION CHARACTERISTICS AS TRANSACTION READ ONLY;");
    }

  }  // namespace ametsuchi
}  // namespace iroha