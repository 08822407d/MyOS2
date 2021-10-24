set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${PROJECT_SOURCE_DIR}/kernel/arch/${ARCH}/kernel.lds")

file(GLOB KERNEL_C_SRCS *.c)
file(GLOB KERNEL_ASM_SRCS *.S)

add_executable(system)
target_sources(system PRIVATE ${KERNEL_C_SRCS})
target_sources(system PRIVATE ${KERNEL_ASM_SRCS})
target_sources(system PRIVATE ${LIB_C_SRCS})
target_sources(system PRIVATE ${LIB_ASM_SRCS})

add_subdirectory(arch)
add_subdirectory(mm)
add_subdirectory(fs)
add_subdirectory(klib)