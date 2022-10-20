file(GLOB ARCH_KERNEL_C_SRCS *.c)
file(GLOB ARCH_KERNEL_ASM_SRCS *.S)

target_sources(kern PRIVATE ${ARCH_KERNEL_C_SRCS} ${ARCH_KERNEL_ASM_SRCS})

add_subdirectory(cpu)
add_subdirectory(apic)