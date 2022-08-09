file(GLOB STUB_C_SRCS *.c)
file(GLOB STUB_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${STUB_C_SRCS} ${STUB_ASM_SRCS})