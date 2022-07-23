file(GLOB LIBX_C_SRCS *.c)
file(GLOB LIBX_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBX_C_SRCS} ${LIBX_ASM_SRCS})