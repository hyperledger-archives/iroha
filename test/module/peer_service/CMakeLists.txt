# SelfState Test
add_executable(self_state_test
        self_state_test.cpp
        )
target_link_libraries(self_state_test
        peer_service
        gtest
        )
add_test(
        NAME self_state_test
        COMMAND $<TARGET_FILE:self_state_test>
)
