file(GLOB KLIB_C_SRCS *.c)
target_sources(kernel PRIVATE ${KLIB_C_SRCS})