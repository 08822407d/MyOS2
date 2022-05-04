file(GLOB INIT_C_SRCS *.c)
# file(GLOB INIT_ASM_SRCS *.S)
target_sources(kern PRIVATE ${INIT_C_SRCS} ${INIT_ASM_SRCS})