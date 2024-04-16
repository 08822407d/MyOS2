cmake_minimum_required(VERSION 3.16)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

project(MyOS2_Kernel ASM C)

# if (CMAKE_HOST_APPLE)
# 	message("${PROMPT} Working on MAC : ${CMAKE_SYSTEM_PROCESSOR}\n")
# 	set (CMAKE_C_COMPILER /opt/local/bin/x86_64-elf-gcc)
# 	set (CMAKE_ASM_COMPILER /opt/local/bin/x86_64-elf-gcc)
# 	set (CMAKE_C_LINKER /opt/local/bin/x86_64-elf-gcc)
# elseif(CMAKE_HOST_UNIX)
# 	message("${PROMPT} Working on UNIX : ${CMAKE_SYSTEM_PROCESSOR}\n")
# 	set (CMAKE_C_COMPILER /usr/bin/gcc)
# 	set (CMAKE_ASM_COMPILER /usr/bin/gcc)
# 	set (CMAKE_C_LINKER usr/bin/ld)
# endif()

#===============================================================================================#
#                                   global options                                              #
#===============================================================================================#
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(ARCH x86_64)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
	set(ARCH aarch64)
endif()

set(DEBUG true)

#===============================================================================================#
#                                   compile options                                             #
#===============================================================================================#
if (DEFINED DEBUG)
	set(RELEASE_FLAG "-DDEBUG")
elseif (DEFINED RELEASE)
	message("${PROMPT} RELEASE")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()

execute_process(COMMAND bash -c "gcc --print-file-name=include | tr -d '\n\r'"
				OUTPUT_VARIABLE CC_INC_PATH)


# kernel common ASM make flags
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} ${UNUSED_MACRO} \
		-D__KERNEL__ \
		-D__ASSEMBLY__ -DASM_FILE \
		")

# kernel common C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
		-m64 -mcmodel=large -fPIC \
		-ffreestanding -nostartfiles \
		-nostdinc -nostdlib \
		-fno-stack-protector \
		-fno-unwind-tables \
		-static \
		-fdata-sections -ffunction-sections \
		-ggdb \
		-D__x86_64__ \
		-mabi=sysv \
		-mno-red-zone \
		")
# define kernel internal option macros
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
		${UNUSED_MACRO} -D__KERNEL__ \
		-DCONFIG_INTEL_X64_GDT_LAYOUT \
		-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
		-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
		-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
		-DCONFIG_SLUB \
		-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
		-DCONFIG_BUG \
		-DGRUB2_BOOTUP_SUPPORT \
		-T ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/kernel.lds \
		")
# message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}



file(GLOB_RECURSE KERNEL_C_SRCS *.c)
file(GLOB_RECURSE KERNEL_ASM_SRCS *.S)

add_executable(kernel)
target_sources(kernel PRIVATE ${KERNEL_C_SRCS} ${KERNEL_ASM_SRCS})
target_include_directories(kernel PRIVATE
				${PROJECT_SOURCE_DIR}
				${PROJECT_SOURCE_DIR}/include
				${PROJECT_SOURCE_DIR}/include/linux/kernel
				${PROJECT_SOURCE_DIR}/arch/${ARCH}
				${PROJECT_SOURCE_DIR}/arch/${ARCH}/include
				${PROJECT_SOURCE_DIR}/../myloader
				)

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

file(COPY "${PROJECT_SOURCE_DIR}/include/uapi" DESTINATION "${CMAKE_BINARY_DIR}/headers")
install(TARGETS kernel DESTINATION boot)
