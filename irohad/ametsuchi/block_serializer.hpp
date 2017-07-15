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

#ifndef IROHA_BLOCK_SERIALIZER_HPP
#define IROHA_BLOCK_SERIALIZER_HPP

#include <model/block.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

namespace iroha{
namespace ametsuchi{

using namespace rapidjson;

class BlockSerializer{
 public:
  std::vector<uint8_t > serialize(model::Block block);
 private:
  void serialize(PrettyWriter<StringBuffer>& writer, model::Block block);
  void serialize(PrettyWriter<StringBuffer>& writer, model::Signature signature);
  void serialize(PrettyWriter<StringBuffer>& writer, model::Transaction transaction);
  void serialize(PrettyWriter<StringBuffer>& writer, model::Command& command);
  template<typename Base, typename T>
  inline bool instanceof(const T *ptr) {
    return typeid(Base) == typeid(*ptr);
  }
};

}
}

#endif //IROHA_BLOCK_SERIALIZER_HPP
