file(GLOB KLIB_C_SRCS *.c)
target_sources(kern PRIVATE ${KLIB_C_SRCS})