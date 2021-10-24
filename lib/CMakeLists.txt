set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${PROJECT_SOURCE_DIR}/kernel/arch/${ARCH}/kernel.lds")

add_subdirectory(libc)