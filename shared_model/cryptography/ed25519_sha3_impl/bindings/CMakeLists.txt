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

if (SWIG)

find_package(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
SET(CMAKE_SWIG_FLAGS "")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET_SOURCE_FILES_PROPERTIES(crypto.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_LIBRARY(crypto LANGUAGE python SOURCES crypto.i)

SWIG_LINK_LIBRARIES(crypto ${PYTHON_LIBRARIES} shared_ed25519_sha3 hash cryptography ed25519_sha3)

endif()
