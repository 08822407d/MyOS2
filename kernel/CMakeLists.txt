set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_KERNEL -T ${PROJECT_SOURCE_DIR}/kernel/arch/${ARCH}/kernel.lds")

file(GLOB KERNEL_C_SRCS *.c)

add_executable(kern)
target_sources(kern PRIVATE ${KERNEL_C_SRCS})

add_subdirectory(arch)
add_subdirectory(mm)
add_subdirectory(fs)
add_subdirectory(block)
add_subdirectory(klib)