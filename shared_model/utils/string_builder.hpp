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

#ifndef IROHA_SHARED_MODEL_STRING_BUILDER_HPP
#define IROHA_SHARED_MODEL_STRING_BUILDER_HPP

#include <string>

namespace shared_model {
  namespace util {
    /**
     * A simple string builder class for building pretty looking strings
     */
    class PrettyStringBuilder {
     public:
      /**
       * Initializes new string with a provided name
       * @param name
       */
      void initString(const std::string &name) { result_.append(name + ": ["); }

      /**
       * Inserts new level marker
       */
      void insertLevel() { result_.append("["); }

      /**
       * Closes new level marker
       */
      void removeLevel() { result_.append("]"); }

      /**
       * Appends new field to string
       */
      void appendField(const std::string &name, const std::string &value) {
        result_.append(name);
        result_.append("=");
        result_.append(value);
        result_.append(", ");
      }

      /**
       * Appends new field to string
       */
      void appendField(const std::string &name) {
        result_.append(name);
        result_.append(" ");
      }

      /**
       * Finishes string construction
       */
      void finalizeString() { result_.append("]"); }

      /**
       * Returns constructed string. For the best result call after
       * finalizeString().
       * @return
       */
      std::string getResult() const { return result_; }

     private:
      std::string result_;
    };
  }
}

#endif  // IROHA_SHARED_MODEL_STRING_BUILDER_HPP
