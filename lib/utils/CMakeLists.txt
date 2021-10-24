file(GLOB UTILS_C_SRCS *.c)
file(GLOB UTILS_ASM_SRCS *.S)

target_sources(mylibc PUBLIC ${UTILS_C_SRCS} ${UTILS_ASM_SRCS})