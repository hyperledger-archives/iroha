# Copyright 2017 Soramitsu Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

addtest(genesis_block_processor_test genesis_block_server/genesis_proc_test.cpp)
target_link_libraries(genesis_block_processor_test
    genesis_block_server
    crypto
    libs_common
    ametsuchi
    ametsuchi_fixture
    )
