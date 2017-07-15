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

#include "ametsuchi/impl/storage_impl.hpp"
#include "ametsuchi/impl/mutable_storage_impl.hpp"
#include "ametsuchi/impl/postgres_wsv_command.hpp"
#include "ametsuchi/impl/postgres_wsv_query.hpp"
#include "ametsuchi/impl/temporary_wsv_impl.hpp"

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
          std::make_unique<PostgresWsvQuery>(*wsv_transaction);
      std::unique_ptr<WsvCommand> executor =
          std::make_unique<PostgresWsvCommand>(*wsv_transaction);

      return std::make_unique<TemporaryWsvImpl>(
          std::move(postgres_connection), std::move(wsv_transaction),
          std::move(wsv), std::move(executor));
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
          std::make_unique<PostgresWsvQuery>(*wsv_transaction);
      std::unique_ptr<WsvCommand> executor =
          std::make_unique<PostgresWsvCommand>(*wsv_transaction);

      auto index = std::make_unique<cpp_redis::redis_client>();
      try {
        index->connect(redis_host_, redis_port_);
      } catch (const cpp_redis::redis_error &e) {
        // TODO log error
        return nullptr;
      }

      return std::make_unique<MutableStorageImpl>(
          block_store_, std::move(index), std::move(postgres_connection),
          std::move(wsv_transaction), std::move(wsv), std::move(executor));
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
          std::make_unique<PostgresWsvQuery>(*wsv_transaction);

      return std::unique_ptr<StorageImpl>(
          new StorageImpl(block_store_dir, redis_host, redis_port,
                          postgres_options, std::move(block_store),
                          std::move(index), std::move(postgres_connection),
                          std::move(wsv_transaction), std::move(wsv)));
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
        std::unique_ptr<pqxx::lazyconnection> wsv_connection,
        std::unique_ptr<pqxx::nontransaction> wsv_transaction,
        std::unique_ptr<WsvQuery> wsv)
        : block_store_dir_(block_store_dir),
          redis_host_(redis_host),
          redis_port_(redis_port),
          postgres_options_(postgres_options),
          block_store_(std::move(block_store)),
          index_(std::move(index)),
          wsv_connection_(std::move(wsv_connection)),
          wsv_transaction_(std::move(wsv_transaction)),
          wsv_(std::move(wsv)) {
      wsv_transaction_->exec(init_);
      wsv_transaction_->exec(
          "SET SESSION CHARACTERISTICS AS TRANSACTION READ ONLY;");
    }

    rxcpp::observable<model::Transaction> StorageImpl::get_account_transactions(
        ed25519::pubkey_t pub_key) {
      return rxcpp::observable<>::create<model::Transaction>([](auto s) {
        s.on_next(model::Transaction{});
        s.on_completed();
      });
    }

    rxcpp::observable<model::Transaction> StorageImpl::get_asset_transactions(
        std::string asset_full_name) {
      return rxcpp::observable<>::create<model::Transaction>([](auto s) {
        s.on_next(model::Transaction{});
        s.on_completed();
      });
    }

    rxcpp::observable<model::Transaction>
    StorageImpl::get_account_asset_transactions(std::string account_id,
                                                std::string asset_id) {
      return rxcpp::observable<>::create<model::Transaction>([](auto s) {
        s.on_next(model::Transaction{});
        s.on_completed();
      });
    }

    rxcpp::observable<model::Block> StorageImpl::get_blocks_in_range(
        uint32_t from, uint32_t to) {
      return rxcpp::observable<>::create<model::Block>([](auto s) {
        s.on_next(model::Block{});
        s.on_completed();
      });
    }

    nonstd::optional<model::Account> StorageImpl::getAccount(const std::string &account_id) {
      std::shared_lock<std::shared_timed_mutex> write(rw_lock_);
      return wsv_->getAccount(account_id);
    }

    std::vector<ed25519::pubkey_t> StorageImpl::getSignatories(
        const std::string &account_id) {
      std::shared_lock<std::shared_timed_mutex> write(rw_lock_);
      return wsv_->getSignatories(account_id);
    }

    nonstd::optional<model::Asset> StorageImpl::getAsset(const std::string &asset_id) {
      std::shared_lock<std::shared_timed_mutex> write(rw_lock_);
      return wsv_->getAsset(asset_id);
    }

    nonstd::optional<model::AccountAsset> StorageImpl::getAccountAsset(
        const std::string &account_id, const std::string &asset_id) {
      std::shared_lock<std::shared_timed_mutex> write(rw_lock_);
      return wsv_->getAccountAsset(account_id, asset_id);
    }

    std::vector<model::Peer> StorageImpl::getPeers() {
      std::shared_lock<std::shared_timed_mutex> write(rw_lock_);
      return wsv_->getPeers();
    }

  }  // namespace ametsuchi
}  // namespace iroha