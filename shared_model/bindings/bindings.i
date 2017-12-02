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

%module irohalib

%include "std_string.i"
%include "stdint.i"

%rename(prototx) shared_model::proto::Transaction;

%{
#include "builders/protobuf/proto_transaction_builder.hpp"
%}

%include "interfaces/common_objects/types.hpp"
%include "interfaces/transaction.hpp"
%include "interfaces/hashable.hpp"
%include "backend/protobuf/transaction.hpp"
%include "builders/protobuf/proto_transaction_builder.hpp"


namespace shared_model {
  namespace proto {
    %template(tb7) shared_model::proto::TemplateTransactionBuilder<7>;
  }
}
