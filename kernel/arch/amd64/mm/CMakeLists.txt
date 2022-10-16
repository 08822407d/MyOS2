file(GLOB ARCH_MM_C_SRCS *.c)
file(GLOB ARCH_MM_ASM_SRCS *.S)

target_sources(kern PRIVATE ${ARCH_MM_C_SRCS} ${ARCH_MM_ASM_SRCS})