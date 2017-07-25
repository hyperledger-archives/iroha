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

#include "consensus/yac/impl/timer_impl.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      TimerImpl::TimerImpl()
          : valid_(true), thread_(&TimerImpl::worker, this), done_(false) {}

      void TimerImpl::invokeAfterDelay(uint64_t millis,
                                       std::function<void()> handler) {
        deny();
        std::function<void()> task = [this, millis, handler]() {
          std::unique_lock<std::mutex> lock(mtx_);
          if (std::cv_status::timeout ==
              cv_.wait_for(lock, std::chrono::milliseconds(millis))) {
            handler();
          }
        };
        std::lock_guard<std::mutex> lock(t_mtx_);
        task_ = std::make_unique<std::function<void()>>(task);
        t_cv_.notify_one();
      }

      void TimerImpl::deny() { cv_.notify_one(); }

      TimerImpl::~TimerImpl() {
        deny();
        done_ = true;
        {
          std::lock_guard<std::mutex> lock(t_mtx_);
          valid_ = false;
          t_cv_.notify_all();
        }
        if (thread_.joinable()) {
          thread_.join();
        }
      }

      void TimerImpl::worker() {
        while (!done_) {
          std::unique_ptr<std::function<void()>> task;
          if (waitPop(task)) {
            (*task)();
          }
        }
      }

      bool TimerImpl::waitPop(std::unique_ptr<std::function<void()>> &out) {
        std::unique_lock<std::mutex> lock(t_mtx_);
        t_cv_.wait(lock, [this]() { return task_ || !valid_; });
        if (!valid_) {
          return false;
        }
        out = std::move(task_);
        return true;
      }

    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha