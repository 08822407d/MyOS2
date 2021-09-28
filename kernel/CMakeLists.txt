set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${PROJECT_SOURCE_DIR}/kernel/arch/${ARCH}/kernel.lds")

file(GLOB KERNEL_C_SRCS ./*.c)
file(GLOB KERNEL_ASM_SRCS ./*.S)

file(GLOB K_ARCH_C_SRCS ./arch/${ARCH}/*.c)
file(GLOB K_ARCH_ASM_SRCS ./arch/${ARCH}/*.S)

file(GLOB K_LIB_C_SRCS ./klib/*.c)

add_executable(system ${K_ARCH_ASM_SRCS} ${K_ARCH_C_SRCS} ${KERNEL_ASM_SRCS} ${K_LIB_C_SRCS} ${KERNEL_C_SRCS}
                ${LIB_C_SRCS} ${LIB_ASM_SRCS})