/*
Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
http://soramitsu.co.jp
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
     http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef IROHA_FLATBUFFER_SERVICE_H
#define IROHA_FLATBUFFER_SERVICE_H

#include <utils/expected.hpp>
#include "flatbuf/nested_flatbuffer_service.h"

namespace iroha {
struct Transaction;
struct ConsensusEvent;
}

namespace flatbuffer_service {

// namespace autogen_extend {
flatbuffers::Offset<void> CreateCommandDirect(
    flatbuffers::FlatBufferBuilder &_fbb, const void *obj,
    int /* Command */ type);  // TODO: Use scoped enum ::iroha::Command
// } // namespace autogen_extend

Expected<flatbuffers::Offset<::iroha::ConsensusEvent>> copyConsensusEvent(
    flatbuffers::FlatBufferBuilder &fbb, const ::iroha::ConsensusEvent &);

template <typename T>
VoidHandler ensureNotNull(T *value) {
  if (value == nullptr) {
    return makeUnexpected(
        exception::connection::NullptrException(typeid(T).name()));
  }
  return {};
}

std::string toString(const iroha::Transaction &tx);

flatbuffers::unique_ptr_t addSignature(const iroha::ConsensusEvent &event,
                                       const std::string &publicKey,
                                       const std::string &signature);

Expected<flatbuffers::unique_ptr_t> toConsensusEvent(const iroha::Transaction &tx);

flatbuffers::unique_ptr_t makeCommit(const iroha::ConsensusEvent &event);
};
#endif  // IROHA_FLATBUFFER_SERVICE_H
