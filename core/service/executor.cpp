/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
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

#include "executor.hpp"

#include <model/commands/add.hpp>
#include <model/commands/transfer.hpp>
#include <model/commands/update.hpp>
#include <model/commands/batch.hpp>
#include <model/commands/unbatch.hpp>
#include <model/commands/contract.hpp>

using namespace command;

template<>
void Executor::execute(Add* add){

}
template<>
void Executor::execute(Transfere* transfer){

}
template<>
void Executor::execute(Update* update){

}
template<>
void Executor::execute(Remove* update){

}
template<>
void Executor::execute(Batch* batch){

}
template<>
void Executor::execute(Unbatch* unbatch){

}
template<>
void Executor::execute(Contract* contract){

}