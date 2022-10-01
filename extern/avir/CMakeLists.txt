# Extra cmake-file for avir since the original repo has none

set(SOURCES
    src/avir_dil.h
    src/avir_float4_sse.h
    src/avir_float8_avx.h
    src/avir.h
    src/lancir.h
)

add_library(avir
    STATIC
    ${SOURCES}
)

set_target_properties(avir
PROPERTIES
    LINKER_LANGUAGE CXX
)
