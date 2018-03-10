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

#include "multi_sig_transactions/state/mst_state.hpp"

#include <utility>

#include "backend/protobuf/transaction.hpp"
#include "common/set.hpp"

namespace iroha {

  // ------------------------------| public api |-------------------------------

  MstState MstState::empty(const CompleterType &completer) {
    return MstState(completer);
  }

  MstState MstState::operator+=(const DataType &rhs) {
    auto result = MstState::empty(completer_);
    insertOne(result, rhs);
    return result;
  }

  MstState MstState::operator+=(const MstState &rhs) {
    auto result = MstState::empty(completer_);
    for (auto &&rhs_tx : rhs.internal_state_) {
      insertOne(result, rhs_tx);
    }
    return result;
  }

  MstState MstState::operator-(const MstState &rhs) const {
    return MstState(this->completer_,
                    set_difference(this->internal_state_, rhs.internal_state_));
  }

  bool MstState::operator==(const MstState &rhs) const {
    return std::is_permutation(
        internal_state_.begin(),
        internal_state_.end(),
        rhs.internal_state_.begin(),
        [](auto tx1, auto tx2) {
          if (*tx1 == *tx2) {
            return std::is_permutation(
                tx1->signatures().begin(),
                tx1->signatures().end(),
                tx2->signatures().begin(),
                [](auto sig1, auto sig2) { return sig1 == sig2; });
          }
          return false;
        });
  }

  bool MstState::isEmpty() const {
    return internal_state_.empty();
  }

  std::vector<DataType> MstState::getTransactions() const {
    return std::vector<DataType>(internal_state_.begin(),
                                 internal_state_.end());
  }

  MstState MstState::eraseByTime(const TimeType &time) {
    MstState out = MstState::empty(completer_);
    while (not index_.empty() and (*completer_)(index_.top(), time)) {
      auto iter = internal_state_.find(index_.top());

      out += *iter;
      internal_state_.erase(iter);
      index_.pop();
    }
    return out;
  }

  // ------------------------------| private api |------------------------------

  MstState::MstState(const CompleterType &completer)
      : MstState(completer, InternalStateType{}) {}

  MstState::MstState(const CompleterType &completer,
                     const InternalStateType &transactions)
      : completer_(completer),
        internal_state_(transactions.begin(), transactions.end()),
        index_(transactions.begin(), transactions.end()) {
    log_ = logger::log("MstState");
  }

  void MstState::insertOne(MstState &out_state, const DataType &rhs_tx) {
    auto corresponding = internal_state_.find(rhs_tx);
    if (corresponding == internal_state_.end()) {
      // when state not contains transaction
      rawInsert(rhs_tx);
      return;
    }

    const auto &found = *corresponding;
    auto raw_tx =
        static_cast<shared_model::proto::Transaction &>(*found).getTransport();

    // Find the signature difference between to txes
    std::vector<iroha::Wrapper<shared_model::interface::Signature>> diff;
    std::copy_if(rhs_tx->signatures().begin(),
                 rhs_tx->signatures().end(),
                 std::back_inserter(diff),
                 [&found](auto &sig) {
                   return found->signatures().find(sig)
                       == found->signatures().end();
                 });

    // Append new signatures
    for (auto &&sig : diff) {
      // TODO (@l4l) 04/03/18 simplify with IR-1040
      *raw_tx.add_signature() =
          static_cast<const shared_model::proto::Signature &>(*sig.operator->())
              .getTransport();
    }

    auto tx =
        std::make_shared<shared_model::proto::Transaction>(std::move(raw_tx));
    internal_state_.erase(corresponding);
    internal_state_.insert(tx);

    if ((*completer_)(tx)) {
      // state already has completed transaction,
      // remove from state and return it
      out_state += tx;
      internal_state_.erase(internal_state_.find(tx));
    }
  }

  void MstState::rawInsert(const DataType &rhs_tx) {
    internal_state_.insert(rhs_tx);
    index_.push(rhs_tx);
  }

}  // namespace iroha
