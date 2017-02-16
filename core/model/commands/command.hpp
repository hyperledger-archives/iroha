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
#ifndef IROHA_COMMAND_H
#define IROHA_COMMAND_H

#include <service/executor.hpp>
#include <util/exception.hpp>

#include <model/commands/add.hpp>
#include <model/commands/batch.hpp>
#include <model/commands/contract.hpp>
#include <model/commands/remove.hpp>
#include <model/commands/transfer.hpp>
#include <model/commands/unbatch.hpp>
#include <model/commands/update.hpp>

#include <iostream>
namespace command {

    using object::Object;

    enum class CommandValueT : std::uint8_t {
        null = 0,
        add,
        batch,
        contract,
        remove,
        transfer,
        unbatch,
        update
    };

    inline const char *EnumNamesCommandValue(CommandValueT type) {
        std::cout <<"EnumNamesCommandValue type: "<< (int)type << std::endl;
        switch(type){
            case CommandValueT::null:    return "Null";
            case CommandValueT::add :    return "Add";
            case CommandValueT::batch:   return "Batch";
            case CommandValueT::contract:return "Contract";
            case CommandValueT::remove:  return "Remove";
            case CommandValueT::transfer:return "Transfer";
            case CommandValueT::unbatch: return "Unbatch";
            case CommandValueT::update:  return "Update";
            default:
                throw exception::NotImplementedException(
                        "Unknown value in EnumNamesCommandValue!",__FILE__
                );
        };
    }

    // There is kind of Currency, Asset,Domain,Account,Message and Peer. Associate SmartContract with Asset.
    struct Command {
        Add*            add;
        Batch*          batch;
        Contract*       contract;
        Remove*         remove;
        Transfer*       transfer;
        Unbatch*        unbatch;
        Update*         update;

        const CommandValueT commandType;

        Command();
        Command(const Add& rhs);
        Command(const Batch& rhs);
        Command(const Contract& rhs);
        Command(const Remove& rhs);
        Command(const Transfer& rhs);
        Command(const Unbatch& rhs);
        Command(const Update& rhs);

        void execute(Executor&);
        CommandValueT getCommandType() const;
        std::string getHash();
        Object getObject() const;

        Add*            AsAdd();
        Batch*          AsBatch();
        Contract*       AsContract();
        Remove*         AsRemove();
        Transfer*       AsTransfer();
        Unbatch*        AsUnbatch();
        Update*         AsUpdate();
    };
};

#endif //IROHA_COMMAND_H
