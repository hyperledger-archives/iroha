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
#include "command.hpp"

namespace command {

    // There is kind of Currency, Asset,Domain,Account,Message and Peer. Associate SmartContract with Asset.
    Command::Command():
            commandType(CommandValueT::null)
    {};	// ctor for ValueT::null

    Command::Command(const Add& rhs):
            commandType(CommandValueT::add)
    {
        add = new Add(rhs);
    }
    Command::Command(const Batch& rhs):
            commandType(CommandValueT::batch)
    {
        batch = new Batch(rhs);
    }
    Command::Command(const Contract& rhs):
            commandType(CommandValueT::contract)
    {
        contract = new Contract(rhs);
    }
    Command::Command(const Remove& rhs):
            commandType(CommandValueT::remove)
    {
        remove = new Remove(rhs);
    }
    Command::Command(const Transfer& rhs):
            commandType(CommandValueT::transfer)
    {
        transfer = new Transfer(rhs);
    }
    Command::Command(const Unbatch& rhs):
            commandType(CommandValueT::unbatch)
    {
        unbatch = new Unbatch(rhs);
    }
    Command::Command(const Update& rhs):
            commandType(CommandValueT::update)
    {
        update = new Update(rhs);
    }

    Add*        Command::AsAdd(){
        return commandType == CommandValueT::add?
               add : nullptr;
    }

    Batch*      Command::AsBatch(){
        return commandType == CommandValueT::batch?
               batch : nullptr;
    }
    Contract*   Command::AsContract(){
        return commandType == CommandValueT::contract?
               contract : nullptr;
    }
    Remove*     Command::AsRemove(){
        return commandType == CommandValueT::remove?
               remove : nullptr;
    }
    Transfer*   Command::AsTransfer(){
        return commandType == CommandValueT::transfer?
               transfer : nullptr;
    }
    Unbatch*    Command::AsUnbatch(){
        return commandType == CommandValueT::unbatch?
               unbatch : nullptr;
    }
    Update*     Command::AsUpdate(){
        return commandType == CommandValueT::update?
               update : nullptr;
    }

    Object Command::getObject() const{
        switch (commandType) {
            case CommandValueT::add: {
                return add->object;
            }
            case CommandValueT::batch: {
                logger::fatal("model command") << "batch dose not have object";
                exit(EXIT_FAILURE);
            }
            case CommandValueT::contract: {
                return contract->object;
            }
            case CommandValueT::remove: {
                return remove->object;
            }
            case CommandValueT::transfer: {
                return transfer->object;
            }
            case CommandValueT::unbatch: {
                logger::fatal("model command") << "unbatch dose not have object";
                exit(EXIT_FAILURE);
            }
            case CommandValueT::update: {
                return update->object;
                break;
            }
            case CommandValueT::null: {
                logger::fatal("model command") << "null dose not have object";
                exit(EXIT_FAILURE);
            }

            default: {
                logger::fatal("model object") << "Unexpected ValueT: " << static_cast<std::uint8_t>(commandType);
                exit(EXIT_FAILURE);
            }
        }
    }

    void Command::execute(Executor& e){

    }

    CommandValueT Command::getCommandType() const {
        return this->commandType;
    }

    std::string Command::getHash(){
        return "WIP";
    }
}