file(GLOB LIBIO_C_SRCS *.c)
file(GLOB LIBIO_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBIO_C_SRCS} ${LIBIO_ASM_SRCS})