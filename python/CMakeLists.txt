find_package(Python 3.6 COMPONENTS Interpreter Development REQUIRED)

pybind11_add_module(pyvecx_rl
    MODULE
    THIN_LTO
    pybind_vecx_rl.cpp
)

target_link_libraries(pyvecx_rl
    PUBLIC vecx_rl
)

target_include_directories(pyvecx_rl
    PRIVATE ${PROJECT_SOURCE_DIR}/environment
)
