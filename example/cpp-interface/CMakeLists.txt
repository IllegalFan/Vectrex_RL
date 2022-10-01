add_executable(test_cpp 
    EXCLUDE_FROM_ALL
    test_vecx_rl.cpp
)

target_link_libraries(test_cpp PUBLIC vecx_rl)

target_include_directories(test_cpp PRIVATE ${PROJECT_SOURCE_DIR}/environment )
