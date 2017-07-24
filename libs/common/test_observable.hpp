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

#ifndef IROHA_TEST_OBSERVABLE_HPP
#define IROHA_TEST_OBSERVABLE_HPP

#include <rxcpp/rx.hpp>
#include <functional>
#include <memory>
#include <utility>

namespace common {
  namespace test_observable {

    /**
     * Interface of testing verification uses in test subscriber;
     * @tparam T type of observed data
     *
     * For implement own strategies should:
     *  1) inherit from this class
     *  2) implement move constructor and move operator=
     *  3) deny copy constructor and operator=
     *  4) implement on_next_* handlers
     *  5) implement validate() method
     */
    template<typename T>
    class VerificationStrategy {
     public:

      /**
       * Handler that called before providing value to target subscriber
       * @param next - new value
       */
      virtual void on_next_before(T next) {
      };

      /**
       * Handler that called after calling target subscriber
       * @param next - new value
       */
      virtual void on_next_after(T next) {
      };

      /**
       * Implement destructor for verify invariant
       */
      virtual ~VerificationStrategy() noexcept(false) {

      };

      /**
       * validation function for invariant
       * @return true if invariant safe, otherwice false
       */
      virtual bool validate() = 0;
     protected:

      /**
       * Exception reason
       */
      std::string invalidate_reason_ = "empty_reason";
    };

    /**
     * TestObservable class provide wrapper for observable
     * @tparam T type of data in wrapped observable
     */
    template<typename T>
    class TestObservable {
     public:

      /**
       * Constructor for wrapping observable for checking invariant
       * @param unwrapped_observable - object for wrapping
       */
      TestObservable(rxcpp::observable<T> unwrapped_observable)
          : unwrapped_(unwrapped_observable) {
      };

      /**
       * Method provide subscription
       * for wrapped observable with checking invariant.
       * @param strategy - invariant for validation
       * @param subscriber - business logic subscriber
       */
      void test_subscriber(std::unique_ptr<VerificationStrategy<T>> strategy,
                           std::function<void(T val)> subscriber) {
        strategy_ = std::move(strategy);
        unwrapped_.subscribe([this, subscriber](T val) {
          // verify before invariant
          this->strategy_->on_next_before(val);

          // invoke subscriber
          subscriber(val);

          // verify after invariant
          this->strategy_->on_next_after(val);
        });
      };

      /**
       * Validate invariant
       * @return true if invariant correct
       */
      bool validate() {
        return strategy_->validate();
      }

     private:
      rxcpp::observable<T> unwrapped_;
      std::unique_ptr<VerificationStrategy<T>> strategy_;
    };

    /**
     * CallExact check invariant that subscriber called exact number of timers
     * @tparam T - observable parameter
     */
    template<typename T>
    class CallExact : public VerificationStrategy<T> {
     public:

      /**
       * @param expected_number_of_calls - number of calls
       * that required for call
       */
      CallExact(uint64_t expected_number_of_calls) :
          expected_number_of_calls_(expected_number_of_calls) {
      };

      CallExact(CallExact<T> &&rhs) {
        number_of_calls_ = 0;
        expected_number_of_calls_ = 0;
        std::swap(expected_number_of_calls_, rhs.expected_number_of_calls_);
        std::swap(number_of_calls_, rhs.number_of_calls_);
      }

      CallExact<T> &operator=(CallExact<T> &&rhs) {
        number_of_calls_ = 0;
        expected_number_of_calls_ = 0;
        std::swap(expected_number_of_calls_, rhs.expected_number_of_calls_);
        std::swap(number_of_calls_, rhs.number_of_calls_);
      }

      /**
       * Remove copy operator=
       */
      CallExact<T> &operator=(CallExact<T> &rhs) = delete;

      /**
       * Remove copy constructor
       */
      CallExact(const CallExact<T> &rhs) = delete;

      void on_next_after(T next) override {
        ++number_of_calls_;
      };

      virtual bool validate() {
        auto val = number_of_calls_ == expected_number_of_calls_;
        if (!val) {
          this->invalidate_reason_ = "Expected calls: " +
              std::to_string(expected_number_of_calls_) +
              ", but called " +
              std::to_string(number_of_calls_);
        }
        return val;
      };

     private:
      uint64_t expected_number_of_calls_;
      uint64_t number_of_calls_ = 0;
    };

  } // namespace test_observable
} // namespace common
#endif //IROHA_TEST_OBSERVABLE_HPP
