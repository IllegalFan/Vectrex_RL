cmake_minimum_required (VERSION 3.21)

project (
    Vectrex_RL
    LANGUAGES CXX C
    VERSION 0.1
)

#################
# Configuration #
#################

# Set c++ Standard
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# vectrex emulator
add_subdirectory(vecx)

add_subdirectory(environment)
