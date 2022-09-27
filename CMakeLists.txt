cmake_minimum_required (VERSION 3.21)

project (
    Vectrex_RL
    LANGUAGES CXX C
    VERSION 1.0
)

#################
# Configuration #
#################

# Set c++ Standard
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#######################
# Extern dependencies #
#######################

# library required for image downsampling
add_subdirectory(extern/avir)

# python wrapper for c++11 and upwards
add_subdirectory(extern/pybind11)

##################
# vecx_rl source #
##################

# vectrex emulator
add_subdirectory(vecx)

# environment for reinforcement learning agents
add_subdirectory(environment)

# python interface for vecx_rl
add_subdirectory(python)
