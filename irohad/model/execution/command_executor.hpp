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

#ifndef IROHA_COMMAND_EXECUTOR_HPP
#define IROHA_COMMAND_EXECUTOR_HPP

#include <boost/format.hpp>

#include "ametsuchi/wsv_command.hpp"
#include "ametsuchi/wsv_query.hpp"
#include "common/result.hpp"
#include "model/command.hpp"

namespace iroha {
  namespace model {

    /**
     * Error message contains command name, as well as what went wrong
     * it may be validation error, or database error
     */
    struct ExecutionError {
      std::string command_name;
      std::string error_message;

      std::string toString() const {
        return (boost::format("%s: %s") % command_name % error_message).str();
      }
    };

    using ExecutionResult = expected::Result<void, ExecutionError>;

    /**
     * Encapsulates validation and execution logic for the command
     */
    class CommandExecutor {
     public:
      /**
       * Check permissions and perform stateful validation
       * @param command - command to be validated
       * @param queries - world state view query interface
       * @param creator - transaction creators account
       * @return true, if validation is successful
       */
      bool validate(const Command &command,
                    ametsuchi::WsvQuery &queries,
                    const std::string &creator_account_id);

      /**
       * Execute the command on the world state view
       * @param command - command to be executed
       * @param queries - world state view query interface
       * @param commands - world state view command interface
       * @return true, if execution is successful
       */
      virtual ExecutionResult execute(
          const Command &command,
          ametsuchi::WsvQuery &queries,
          ametsuchi::WsvCommand &commands,
          const std::string &creator_account_id) = 0;

      virtual ~CommandExecutor() = default;

     protected:
      /**
       * Check permission for given creator account
       * @param command - command to be validated
       * @param queries - world state view query interface
       * @param creator - transaction creators account
       * @return true, if validation is successful
       */
      virtual bool hasPermissions(const Command &command,
                                  ametsuchi::WsvQuery &queries,
                                  const std::string &creator_account_id) = 0;

      /**
       * Perform stateful validation for the command
       * @param command - command to be validated
       * @param queries - world state view query interface
       * @return true, if command is valid
       */
      virtual bool isValid(const Command &command,
                           ametsuchi::WsvQuery &queries,
                           const std::string &creator_account_id) = 0;

      /**
       * String name of the command
       * @return name of the command for which executor is invoked
       */
      virtual std::string commandName() const noexcept = 0;

      /**
       * Creates Error out of given error message
       * @param error_message what message should be displayed to the user
       * @return Error object which can be used for result construction
       */
      expected::Error<ExecutionError> makeExecutionError(
          const std::string &error_message) const noexcept;

      ExecutionResult errorIfNot(bool predicate,
                                 const std::string &error_message) const
          noexcept;
    };

    class AppendRoleExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class DetachRoleExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class CreateRoleExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class GrantPermissionExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class RevokePermissionExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class AddAssetQuantityExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class SubtractAssetQuantityExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class AddPeerExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class AddSignatoryExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class CreateAccountExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class CreateAssetExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class CreateDomainExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class RemoveSignatoryExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class SetAccountDetailExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class SetQuorumExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

    class TransferAssetExecutor : public CommandExecutor {
     public:
      ExecutionResult execute(const Command &command,
                              ametsuchi::WsvQuery &queries,
                              ametsuchi::WsvCommand &commands,
                              const std::string &creator_account_id) override;

     protected:
      bool hasPermissions(const Command &command,
                          ametsuchi::WsvQuery &queries,
                          const std::string &creator_account_id) override;

      bool isValid(const Command &command,
                   ametsuchi::WsvQuery &queries,
                   const std::string &creator_account_id) override;

      std::string commandName() const noexcept override;
    };

  }  // namespace model
}  // namespace iroha

#endif  // IROHA_COMMAND_EXECUTOR_HPP
