set(SOURCES
    environment.hpp
    environment.cpp
    action.hpp
    action.cpp
    image.hpp
    image.cpp
    games/ROM.hpp
    games/ROM.cpp
    games/unsupported_rom.hpp
    games/roms.hpp
    games/frog_jump.hpp
    games/frog_jump.cpp
)

add_library(vecx_rl
    STATIC
    ${SOURCES}
)

target_include_directories(vecx_rl
    PUBLIC ../vecx
    PUBLIC ./games
    PUBLIC ../extern/avir/src
)

target_link_libraries(vecx_rl
    SDL2
    SDL2_gfx
    pthread
    vecx
    avir
)

cmake_path(SET VECX_ROM_REL "${PROJECT_SOURCE_DIR}/vecx/rom_noIntro.dat")
cmake_path(ABSOLUTE_PATH VECX_ROM_REL OUTPUT_VARIABLE VECX_ROM_ABS)

cmake_path(SET ROM_BASEDIR_REL "${PROJECT_SOURCE_DIR}/res/vectrex_roms/")
cmake_path(ABSOLUTE_PATH ROM_BASEDIR_REL OUTPUT_VARIABLE ROM_BASEDIR_ABS)

target_compile_definitions(vecx_rl
    PRIVATE VECX_ROM="${VECX_ROM_ABS}"
    PRIVATE ROM_BASEDIR="${ROM_BASEDIR_ABS}"
)
