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

#include "module/irohad/ametsuchi/ametsuchi_mocks.hpp"
#include "module/irohad/multi_sig_transactions/mst_mocks.hpp"
#include "module/irohad/network/network_mocks.hpp"
#include "module/irohad/torii/torii_mocks.hpp"
#include "module/irohad/validation/validation_mocks.hpp"

#include "backend/protobuf/from_old_model.hpp"
#include "builders/protobuf/common_objects/proto_signature_builder.hpp"
#include "builders/protobuf/transaction.hpp"
#include "framework/test_subscriber.hpp"
#include "model/transaction_response.hpp"
#include "torii/processor/transaction_processor_impl.hpp"

using namespace iroha;
using namespace iroha::network;
using namespace iroha::validation;
using namespace iroha::torii;
using namespace iroha::model;
using namespace iroha::ametsuchi;
using namespace framework::test_subscriber;

using ::testing::A;
using ::testing::Return;
using ::testing::_;

class TransactionProcessorTest : public ::testing::Test {
 public:
  void SetUp() override {
    pcs = std::make_shared<MockPeerCommunicationService>();
    mp = std::make_shared<MockMstProcessor>();
    rxcpp::subjects::subject<std::shared_ptr<shared_model::interface::Proposal>>
        prop_notifier;
    rxcpp::subjects::subject<Commit> commit_notifier;

    EXPECT_CALL(*pcs, on_proposal())
        .WillRepeatedly(Return(prop_notifier.get_observable()));
    EXPECT_CALL(*pcs, on_commit())
        .WillRepeatedly(Return(commit_notifier.get_observable()));

    EXPECT_CALL(*mp, onPreparedTransactionsImpl())
        .WillRepeatedly(Return(mst_prepared_notifier.get_observable()));
    EXPECT_CALL(*mp, onExpiredTransactionsImpl())
        .WillRepeatedly(Return(mst_expired_notifier.get_observable()));

    tp = std::make_shared<TransactionProcessorImpl>(pcs, mp);
  }

  auto base_tx() {
    return shared_model::proto::TransactionBuilder()
        .creatorAccountId("user@domain")
        .createdTime(iroha::time::now())
        .txCounter(1)
        .setAccountQuorum("user@domain", 2)
        .quorum(1);
  }

  rxcpp::subjects::subject<iroha::DataType> mst_prepared_notifier;
  rxcpp::subjects::subject<iroha::DataType> mst_expired_notifier;

  std::shared_ptr<MockPeerCommunicationService> pcs;
  std::shared_ptr<TransactionProcessorImpl> tp;
  std::shared_ptr<MockMstProcessor> mp;
};

/**
 * @given simple valid tx
 * @when transaction_processor handle it
 * @then it returns STATELESS_VALIDATION_SUCCESS
 */
TEST_F(TransactionProcessorTest, ValidTransaction) {
  EXPECT_CALL(*mp, propagateTransactionImpl(_)).Times(0);
  EXPECT_CALL(*pcs, propagate_transaction(_)).Times(1);

  auto tx = base_tx().build().signAndAddSignature(
      shared_model::crypto::DefaultCryptoAlgorithmType::generateKeypair());

  auto wrapper = make_test_subscriber<CallExact>(tp->transactionNotifier(), 1);
  wrapper.subscribe([](auto response) {
    auto resp = static_cast<TransactionResponse &>(*response);
    ASSERT_EQ(resp.current_status,
              TransactionResponse::STATELESS_VALIDATION_SUCCESS);
  });
  tp->transactionHandle(std::shared_ptr<model::Transaction>(tx.makeOldModel()));

  ASSERT_TRUE(wrapper.validate());
}

/**
 * @given valid multisig tx
 * @when transaction_processor handle it
 * @then it goes to mst and after signing goes to PeerCommunicationService
 */
TEST_F(TransactionProcessorTest, MultisigTransaction) {
  std::shared_ptr<shared_model::proto::Transaction> after_mst;
  auto mst_propagate = [&after_mst](
                           std::shared_ptr<shared_model::interface::Transaction>
                               tx) {
    after_mst = std::static_pointer_cast<shared_model::proto::Transaction>(tx);
    auto make_signature = [&after_mst] {
      auto keypair =
          shared_model::crypto::DefaultCryptoAlgorithmType::generateKeypair();
      auto signedBlob = shared_model::crypto::CryptoSigner<>::sign(
          shared_model::crypto::Blob(after_mst->payload()), keypair);
      return shared_model::detail::makePolymorphic<
          shared_model::proto::Signature>(
          shared_model::proto::SignatureBuilder()
              .publicKey(keypair.publicKey())
              .signedData(signedBlob)
              .build());
    };
    after_mst->addSignature(make_signature());
    after_mst->addSignature(make_signature());
  };
  EXPECT_CALL(*mp, propagateTransactionImpl(_))
      .WillOnce(testing::Invoke(mst_propagate));
  EXPECT_CALL(*pcs, propagate_transaction(_)).Times(1);

  auto tx = base_tx().quorum(2).build().signAndAddSignature(
      shared_model::crypto::DefaultCryptoAlgorithmType::generateKeypair());
  std::shared_ptr<model::Transaction> old_tx(tx.makeOldModel());

  auto wrapper = make_test_subscriber<CallExact>(tp->transactionNotifier(), 2);
  wrapper.subscribe([](auto response) {
    auto resp = static_cast<TransactionResponse &>(*response);
    ASSERT_EQ(resp.current_status,
              TransactionResponse::STATELESS_VALIDATION_SUCCESS);
  });
  tp->transactionHandle(old_tx);
  mst_prepared_notifier.get_subscriber().on_next(after_mst);

  ASSERT_TRUE(wrapper.validate());
}

/**
 * @given valid multisig tx
 * @when transaction_processor handle it
 * @then ensure after expiring it leads to MST_EXPIRED status
 */
TEST_F(TransactionProcessorTest, MultisigExpired) {
  EXPECT_CALL(*mp, propagateTransactionImpl(_)).Times(1);
  EXPECT_CALL(*pcs, propagate_transaction(_)).Times(0);

  auto tx = base_tx().quorum(2).build().signAndAddSignature(
      shared_model::crypto::DefaultCryptoAlgorithmType::generateKeypair());
  std::shared_ptr<model::Transaction> old_tx(tx.makeOldModel());

  auto wrapper = make_test_subscriber<CallExact>(tp->transactionNotifier(), 2);
  wrapper.subscribe([](auto response) {
    static int idx = 0;
    auto resp = static_cast<TransactionResponse &>(*response);
    ASSERT_EQ(resp.current_status,
              idx++ == 0 ? TransactionResponse::STATELESS_VALIDATION_SUCCESS
                         : TransactionResponse::MST_EXPIRED);
  });
  tp->transactionHandle(old_tx);
  mst_expired_notifier.get_subscriber().on_next(
      std::make_shared<shared_model::proto::Transaction>(
          shared_model::proto::from_old(*old_tx)));

  ASSERT_TRUE(wrapper.validate());
}
