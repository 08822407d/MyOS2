file(GLOB UTIL_C_SRCS *.c)
file(GLOB UTIL_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${UTIL_C_SRCS} ${UTIL_ASM_SRCS})