# kernel common ASM make flags
set(CMAKE_ASM_FLAGS
		"${CMAKE_ASM_FLAGS} \
		${UNUSED_MACRO} \
		-D__KERNEL__ ")
# kernel common C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
				-mno-red-zone \
				${UNUSED_MACRO} \
				-D__KERNEL__ -DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
				-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
				-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
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