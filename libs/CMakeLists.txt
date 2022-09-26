# Extra cmake-file for avir since the original repo has none

set(SOURCES
avir/avir_dil.h
avir/avir_float4_sse.h
avir/avir_float8_avx.h
avir/avir.h
avir/lancir.h
)

add_library(avir
STATIC
${SOURCES}
)

set_target_properties(avir
PROPERTIES
    LINKER_LANGUAGE CXX
)
