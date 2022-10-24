file(GLOB TEST_C_SRCS *.c)
# file(GLOB TEST_ASM_SRCS *.S)
target_sources(kern PRIVATE ${TEST_C_SRCS} ${TEST_ASM_SRCS})