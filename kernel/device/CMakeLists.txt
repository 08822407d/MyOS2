file(GLOB DEV_C_SRCS *.c)
# file(GLOB DEV_ASM_SRCS *.S)
target_sources(kern PRIVATE ${DEV_C_SRCS} ${DEV_ASM_SRCS})