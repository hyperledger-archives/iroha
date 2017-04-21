/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
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

#include <infra/flatbuf/commands_generated.h>
#include <infra/flatbuf/main_generated.h>
#include <utils/logger.hpp>

#include <memory>
#include <string>

namespace flatbuffer_service {
// namespace nested_flatbuffer_service {

std::vector<uint8_t> CreateAccountBuffer(
    const char* publicKey, const char* alias,
    const std::vector<std::string>& signatories, uint16_t useKeys) {
  if (&signatories != nullptr) {
    flatbuffers::FlatBufferBuilder fbbAccount;

    std::vector<flatbuffers::Offset<flatbuffers::String>> signatoryOffsets;
    for (const auto& e : signatories) {
      signatoryOffsets.push_back(fbbAccount.CreateString(e));
    }

    auto accountOffset = ::iroha::CreateAccountDirect(
        fbbAccount, publicKey, alias, &signatoryOffsets, 1);
    fbbAccount.Finish(accountOffset);

    auto buf = fbbAccount.GetBufferPointer();
    std::vector<uint8_t> buffer;
    buffer.assign(buf, buf + fbbAccount.GetSize());
    return buffer;
  } else {
    flatbuffers::FlatBufferBuilder fbbAccount;
    auto accountOffset =
        ::iroha::CreateAccountDirect(fbbAccount, publicKey, alias, nullptr, 1);
    fbbAccount.Finish(accountOffset);

    auto buf = fbbAccount.GetBufferPointer();
    std::vector<uint8_t> buffer;
    buffer.assign(buf, buf + fbbAccount.GetSize());
    return buffer;
  }
}

// } // namespace nested_flatbuffer_service {
}  // namespace flatbuffer_service
