file(GLOB EXT2_C_SRCS *.c)
file(GLOB EXT2_ASM_SRCS *.S)
target_sources(kernel PRIVATE ${EXT2_C_SRCS} ${EXT2_ASM_SRCS})