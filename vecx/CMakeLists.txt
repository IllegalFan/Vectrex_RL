set(SOURCES
    e6809.h 
    e6809.c 
    e8910.h 
    e8910.c 
    vecx.h 
    vecx.c
    osint.h 
    osint.c
    display.h
    display.c
)

add_library(vecx 
    STATIC
    ${SOURCES}
)

target_compile_definitions(vecx
    PRIVATE D_REENTRANT # recommended if using pthread
)

target_include_directories(vecx
    PUBLIC /usr/include/SDL2
)

# copy vectrex rom
file(COPY rom_noIntro.dat rom.dat
    DESTINATION ${PROJECT_BINARY_DIR}/vecx
    NO_SOURCE_PERMISSIONS
)
