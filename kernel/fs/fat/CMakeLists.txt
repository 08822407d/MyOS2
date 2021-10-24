file(GLOB FAT_C_SRCS *.c)
file(GLOB FAT_ASM_SRCS *.S)
target_sources(kern PRIVATE ${FAT_C_SRCS} ${FAT_ASM_SRCS})