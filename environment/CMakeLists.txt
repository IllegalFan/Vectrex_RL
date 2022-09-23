
add_executable(test
    main.cpp
    environment.hpp
    environment.cpp
    action.hpp
    action.cpp
    image.hpp
    image.cpp
    games/ROM.hpp
    games/ROM.cpp
    games/unsupported_rom.hpp
    # list of supported games
    games/roms.hpp
    games/frog_jump.hpp
    games/frog_jump.cpp
)

target_include_directories(test
    PRIVATE ../vecx
    PRIVATE ./games
    PRIVATE ../libs/avir
)

target_link_libraries(test 
    SDL2
    SDL2_gfx
    pthread
    vecx
    avir
)

target_compile_definitions(test
    PRIVATE DEBUG=0
)
