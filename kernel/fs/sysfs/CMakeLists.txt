file(GLOB SYSFS_C_SRCS *.c)
# file(GLOB VFS_ASM_SRCS *.S)
target_sources(kern PRIVATE ${SYSFS_C_SRCS} ${SYSFS_ASM_SRCS})