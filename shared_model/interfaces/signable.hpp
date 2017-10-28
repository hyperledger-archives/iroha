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

#ifndef IROHA_SIGNABLE_HPP
#define IROHA_SIGNABLE_HPP

#include "interfaces/common_objects/signature.hpp"
#include "interfaces/hashable.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Interface provides signatures and adding them to model object
     * @tparam Model - your new style model
     * Architecture note: we inherit Signable from Hashable with following
     * assumption - all Signable objects are signed by hash value.
     */
    template <typename Model, typename OldModel>
    class Signable : public Hashable<Model, OldModel> {
     public:
      /// Type of transaction signature
      using SignatureType = detail::PolymorphicWrapper<Signature>;

      /// Type of set of signatures
      using SignatureSetType = std::unordered_set<SignatureType>;

      /**
       * @return attached signatures
       */
      virtual const SignatureSetType &signatures() const = 0;

      /**
       * Attach signature to object
       * @param signature - signature object for insertion
       * @return true, if signature was added
       */
      virtual bool addSignature(const SignatureType &signature) = 0;

      /// Type of timestamp
      using TimestampType = uint64_t;

      /**
       * @return time of creation
       */
      virtual const TimestampType &createdTime() const = 0;
    };
  }  // namespace interface
}  // namespace shared_model

namespace std {
  template <>
  struct hash<shared_model::detail::
                  PolymorphicWrapper<shared_model::interface::Signature>> {
    size_t operator()(
        const shared_model::detail::
            PolymorphicWrapper<shared_model::interface::Signature> &sig) const {
      return hash<std::string>()(sig->publicKey().blob()
                                 + sig->signedHash().blob());
    }
  };
}

#endif  // IROHA_SIGNABLE_HPP
