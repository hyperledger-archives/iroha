#ifndef CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
#define CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_

#include "../model/transactions/abstract_transaction.hpp"
#include <memory>
#include <type_traits>

namespace transaction_validator {

    template<typename T, std::enable_if_t<std::is_base_of<abstract_transaction::AbstractTransaction, T>::value, std::nullptr_t> = nullptr>
    bool isValid(const T& tx){
        return true;
    }

    template<typename T, std::enable_if_t<std::is_base_of<abstract_transaction::AbstractTransaction, T>::value, std::nullptr_t> = nullptr>
    bool signaturesAreValid(const T& tx){
        return false;
    }

    template<typename T, std::enable_if_t<std::is_base_of<abstract_transaction::AbstractTransaction, T>::value, std::nullptr_t> = nullptr>
    bool validForType(const T& tx){
        return false;
    }

};  // namespace transaction_validator

#endif  // CORE_VALIDATION_TRANSACTIONVALIDATOR_HPP_
