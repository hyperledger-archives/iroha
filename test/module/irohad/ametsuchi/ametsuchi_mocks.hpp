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

#ifndef IROHA_AMETSUCHI_MOCKS_HPP
#define IROHA_AMETSUCHI_MOCKS_HPP

#include <gmock/gmock.h>
#include <cpp_redis/cpp_redis>
#include <pqxx/pqxx>
#include "ametsuchi/block_query.hpp"
#include "ametsuchi/mutable_factory.hpp"
#include "ametsuchi/mutable_storage.hpp"
#include "ametsuchi/temporary_factory.hpp"
#include "ametsuchi/temporary_wsv.hpp"
#include "ametsuchi/wsv_query.hpp"
#include "ametsuchi/peer_query.hpp"
#include "logger/logger.hpp"
#include "common/files.hpp"

namespace iroha {
  namespace ametsuchi {
    class MockWsvQuery : public WsvQuery {
     public:
      MOCK_METHOD1(getAccount, nonstd::optional<model::Account>(
                                   const std::string &account_id));
      MOCK_METHOD1(getSignatories,
                   nonstd::optional<std::vector<ed25519::pubkey_t>>(
                       const std::string &account_id));
      MOCK_METHOD1(getAsset,
                   nonstd::optional<model::Asset>(const std::string &asset_id));
      MOCK_METHOD2(getAccountAsset, nonstd::optional<model::AccountAsset>(
                                        const std::string &account_id,
                                        const std::string &asset_id));
      MOCK_METHOD0(getPeers, nonstd::optional<std::vector<model::Peer>>());
    };

    class MockWsvCommand : public WsvCommand {
     public:
      MOCK_METHOD1(insertAccount, bool(const model::Account &));
      MOCK_METHOD1(updateAccount, bool(const model::Account &));
      MOCK_METHOD1(insertAsset, bool(const model::Asset &));
      MOCK_METHOD1(upsertAccountAsset, bool(const model::AccountAsset &));
      MOCK_METHOD1(insertSignatory, bool(const ed25519::pubkey_t &));

      MOCK_METHOD2(insertAccountSignatory,
                   bool(const std::string &, const ed25519::pubkey_t &));

      MOCK_METHOD2(deleteAccountSignatory,
                   bool(const std::string &, const ed25519::pubkey_t &));

      MOCK_METHOD1(insertPeer, bool(const model::Peer &));

      MOCK_METHOD1(deletePeer, bool(const model::Peer &));

      MOCK_METHOD1(insertDomain, bool(const model::Domain &));
    };

    class MockBlockQuery : public BlockQuery {
     public:
      MOCK_METHOD1(
          getAccountTransactions,
          rxcpp::observable<model::Transaction>(std::string account_id));
      MOCK_METHOD2(getBlocks,
                   rxcpp::observable<model::Block>(uint32_t, uint32_t));
      MOCK_METHOD1(getBlocksFrom,
                   rxcpp::observable<model::Block>(uint32_t));
      MOCK_METHOD1(getTopBlocks,
                   rxcpp::observable<model::Block>(uint32_t));
    };

    class MockTemporaryFactory : public TemporaryFactory {
     public:
      MOCK_METHOD0(createTemporaryWsv, std::unique_ptr<TemporaryWsv>());
    };

    class MockMutableStorage : public MutableStorage {
     public:
      MOCK_METHOD2(apply,
                   bool(const model::Block &,
                        std::function<bool(const model::Block &,
                                           WsvQuery &, const hash256_t &)>));
      MOCK_METHOD1(getAccount, nonstd::optional<model::Account>(
                                   const std::string &account_id));
      MOCK_METHOD1(getSignatories,
                   nonstd::optional<std::vector<ed25519::pubkey_t>>(
                       const std::string &account_id));
      MOCK_METHOD1(getAsset,
                   nonstd::optional<model::Asset>(const std::string &asset_id));
      MOCK_METHOD2(getAccountAsset, nonstd::optional<model::AccountAsset>(
                                        const std::string &account_id,
                                        const std::string &asset_id));
      MOCK_METHOD0(getPeers, nonstd::optional<std::vector<model::Peer>>());
    };

    /**
     * Factory for generation mock mutable storages.
     * This method provide technique,
     * when required to return object wrapped in unique pointer.
     */
    std::unique_ptr<MutableStorage> createMockMutableStorage() {
      return std::make_unique<MockMutableStorage>();
    }

    class MockMutableFactory : public MutableFactory {
     public:
      MOCK_METHOD0(createMutableStorage, std::unique_ptr<MutableStorage>());

      void commit(std::unique_ptr<MutableStorage> mutableStorage) override {
        // gmock workaround for non-copyable parameters
        commit_(mutableStorage);
      }

      MOCK_METHOD1(commit_, void(std::unique_ptr<MutableStorage> &));
    };

    class MockPeerQuery : public PeerQuery {
     public:
      MockPeerQuery() = default;

      MOCK_METHOD0(getLedgerPeers,
                   nonstd::optional<std::vector<model::Peer>>());
    };

    /**
     * Class with ametsuchi initialization
     */
    class AmetsuchiTest : public ::testing::Test {
     protected:
      virtual void SetUp() {
        auto log = logger::testLog("AmetsuchiTest");

        mkdir(block_store_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        auto pg_host = std::getenv("IROHA_POSTGRES_HOST");
        auto pg_port = std::getenv("IROHA_POSTGRES_PORT");
        auto pg_user = std::getenv("IROHA_POSTGRES_USER");
        auto pg_pass = std::getenv("IROHA_POSTGRES_PASSWORD");
        auto rd_host = std::getenv("IROHA_REDIS_HOST");
        auto rd_port = std::getenv("IROHA_REDIS_PORT");
        if (not pg_host) {
          return;
        }
        std::stringstream ss;
        ss << "host=" << pg_host << " port=" << pg_port << " user=" << pg_user
           << " password=" << pg_pass;
        pgopt_ = ss.str();
        redishost_ = rd_host;
        redisport_ = std::stoull(rd_port);
        log->info("host={}, port={}, user={}, password={}",
                  pg_host, pg_port, pg_user, pg_pass);
      }
      virtual void TearDown() {
        const auto drop =
            "DROP TABLE IF EXISTS account_has_asset;\n"
                "DROP TABLE IF EXISTS account_has_signatory;\n"
                "DROP TABLE IF EXISTS peer;\n"
                "DROP TABLE IF EXISTS account;\n"
                "DROP TABLE IF EXISTS exchange;\n"
                "DROP TABLE IF EXISTS asset;\n"
                "DROP TABLE IF EXISTS domain;\n"
                "DROP TABLE IF EXISTS signatory;";

        pqxx::connection connection(pgopt_);
        pqxx::work txn(connection);
        txn.exec(drop);
        txn.commit();
        connection.disconnect();

        cpp_redis::redis_client client;
        client.connect(redishost_, redisport_);
        client.flushall();
        client.sync_commit();
        client.disconnect();

        iroha::remove_all(block_store_path);
      }

      std::string pgopt_ =
          "host=localhost port=5432 user=postgres password=mysecretpassword";

      std::string redishost_ = "localhost";
      size_t redisport_ = 6379;

      std::string block_store_path = "/tmp/block_store";
    };

  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_AMETSUCHI_MOCKS_HPP
