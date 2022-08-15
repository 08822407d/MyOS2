file(GLOB STDIO_C_SRCS *.c)
file(GLOB STDIO_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${STDIO_C_SRCS} ${STDIO_ASM_SRCS})