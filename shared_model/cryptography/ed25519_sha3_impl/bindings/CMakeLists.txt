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

find_package(SWIG 3.0 REQUIRED)
INCLUDE(${SWIG_USE_FILE})

find_package(PythonLibs 2.7 REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

find_package(JNI REQUIRED)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${JAVA_INCLUDE_PATH2})

set(CMAKE_SWIG_FLAGS "")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties(crypto.i PROPERTIES CPLUSPLUS ON)

swig_add_library(irohacrypto LANGUAGE java SOURCES crypto.i)
swig_link_libraries(irohacrypto ${Java_LIBRARIES} shared_ed25519_sha3 hash cryptography ed25519_sha3)

swig_add_library(irohacrypto LANGUAGE python SOURCES crypto.i)
swig_link_libraries(irohacrypto ${Python_LIBRARIES} shared_ed25519_sha3 hash cryptography ed25519_sha3)

endif()
