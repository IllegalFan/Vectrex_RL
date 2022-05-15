
add_executable(test
    main.cpp
    environment.hpp
    environment.cpp
)

target_include_directories(test
    PRIVATE ../vecx
)

target_link_libraries(test 
    SDL2
    SDL2_gfx
    pthread
    vecx
)
