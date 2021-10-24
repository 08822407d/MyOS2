file(GLOB VFS_C_SRCS *.c)
file(GLOB VFS_ASM_SRCS *.S)
target_sources(kernel PRIVATE ${VFS_C_SRCS} ${VFS_ASM_SRCS})