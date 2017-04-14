/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

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

#include <infra/flatbuf/main_generated.h>

#include <memory>
#include <string>

namespace flatbuffer_service{

    std::string toString(const iroha::Transaction& tx){};

    std::unique_ptr<iroha::ConsensusEvent> toConsensusEvent(const iroha::Transaction& tx){}

    std::unique_ptr<iroha::ConsensusEvent> addSignature(const std::unique_ptr<iroha::ConsensusEvent>& event){}

};