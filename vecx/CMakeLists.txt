set(SOURCES
    e6809.h 
    e6809.c 
    e8910.h 
    e8910.c 
    vecx.h 
    vecx.c
    osint.h 
    osint.c
)

add_library(vecx 
    STATIC
    ${SOURCES}
)

target_compile_definitions(vecx
    PRIVATE D_REENTRANT
)

target_include_directories(vecx
    PRIVATE /usr/include/SDL2
)
