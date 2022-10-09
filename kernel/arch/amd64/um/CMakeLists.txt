file(GLOB ARCH_UM_C_SRCS *.c)
file(GLOB ARCH_UM_ASM_SRCS *.S)

target_sources(kern PRIVATE ${ARCH_UM_C_SRCS} ${ARCH_UM_ASM_SRCS})