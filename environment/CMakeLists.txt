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
    SHARED
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

target_compile_definitions(vecx_rl
    PRIVATE VECX_ROM="../vecx/rom_noIntro.dat"
    PRIVATE ROM_BASEDIR="../vectrex_roms/"
)

# copy game binaries
file(COPY ${PROJECT_SOURCE_DIR}/res/vectrex_roms
    DESTINATION ${PROJECT_BINARY_DIR}/
    NO_SOURCE_PERMISSIONS
)
