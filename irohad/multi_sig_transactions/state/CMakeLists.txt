# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mst_state
    impl/mst_state.cpp
    )

target_link_libraries(mst_state
    mst_hash
    logger
    boost # // TODO [IR-1814] @muratovv 25.10.18: replace with lib dependency
    )
