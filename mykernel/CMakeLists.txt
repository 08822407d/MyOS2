# kernel common ASM make flags
set(CMAKE_ASM_FLAGS
		"${CMAKE_ASM_FLAGS} \
		${UNUSED_MACRO} \
		-D__KERNEL__ ")
# kernel common C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
				-mno-red-zone \
				${UNUSED_MACRO} -D__KERNEL__ \
				-DCONFIG_INTEL_X64_GDT_LAYOUT \
				-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
				-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
				-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
				-DCONFIG_SLUB \
				-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
				-DCONFIG_BUG \
				-DGRUB2_BOOTUP_SUPPORT \
				-T ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/kernel.lds")
# message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS}")

file(GLOB_RECURSE KERNEL_C_SRCS *.c)
file(GLOB_RECURSE KERNEL_ASM_SRCS *.S)

add_executable(kernel)
target_sources(kernel PRIVATE ${KERNEL_C_SRCS} ${KERNEL_ASM_SRCS})
target_include_directories(kernel PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}
				${CMAKE_CURRENT_SOURCE_DIR}/include
				${CMAKE_CURRENT_SOURCE_DIR}/include/linux/kernel
				${PROJECT_SOURCE_DIR}/myloader
				${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}
				${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/include)

add_subdirectory(arch)
add_subdirectory(init)
add_subdirectory(sched)
add_subdirectory(mm)
add_subdirectory(fs)
add_subdirectory(time)
add_subdirectory(device)
add_subdirectory(drivers)
add_subdirectory(block)
add_subdirectory(lib)
add_subdirectory(printk)

add_subdirectory(klib)
add_subdirectory(debug)
add_subdirectory(lock_IPC)
add_subdirectory(kactive)