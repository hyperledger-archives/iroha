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

#ifndef IROHA_LOOKUP3_H
#define IROHA_LOOKUP3_H

#include <stdint.h> // for uint32_t
#include <stddef.h> // for size_t

/// TODO there are more functions, but we use only this

uint32_t hashlittle( const void *key, size_t length, uint32_t initval);

#endif //IROHA_LOOKUP3_H
