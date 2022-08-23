
add_executable(test
    main.cpp
    environment.hpp
    environment.cpp
    action.hpp
    action.cpp
    games/ROM.hpp
    games/ROM.cpp
    games/unsupported_rom.hpp
    # list of supported games
    games/games.hpp
    games/frog_jump.hpp
    games/frog_jump.cpp
)

target_include_directories(test
    PRIVATE ../vecx
    PRIVATE ./games
)

target_link_libraries(test 
    SDL2
    SDL2_gfx
    pthread
    vecx
)
