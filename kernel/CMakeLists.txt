# kernel common C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
				-mno-red-zone \
				-D__KERNEL__ -DCONFIG_FLATMEM -DCONFIG_MAX_CPUS=256 \
				-DCONFIG_64BIT \
				-T ${PROJECT_SOURCE_DIR}/kernel/arch/${ARCH}/kernel.lds")
# message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS}")

file(GLOB KERNEL_C_SRCS *.c)

add_executable(kern)
target_sources(kern PRIVATE ${KERNEL_C_SRCS})
target_include_directories(kern PRIVATE
				${PROJECT_SOURCE_DIR}/kernel/include
				${PROJECT_SOURCE_DIR}/loader)

add_subdirectory(arch)
add_subdirectory(init)
add_subdirectory(sched)
add_subdirectory(mm)
add_subdirectory(fs)
add_subdirectory(block)
add_subdirectory(device)
add_subdirectory(drivers)
add_subdirectory(lib)

add_subdirectory(klib)
add_subdirectory(test)