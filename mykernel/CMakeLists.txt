cmake_minimum_required(VERSION 3.16)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
project(MyOS2_Kernel ASM C)


set(PROMPT "======>>>")
set(PROMPT_END "<<<======")

#===============================================================================================#
#										global options											#
#===============================================================================================#
if (NOT DEFINED ${ARCH})
	set(ARCH x86_64)
endif()
message("${PROMPT} ARCH : ${ARCH} ${PROMPT_END}")


#===============================================================================================#
#                                   compile options                                             #
#===============================================================================================#
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
else ()
	message("${PROMPT} RELEASE  ${PROMPT_END}")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()

execute_process(COMMAND bash -c "gcc --print-file-name=include | tr -d '\n\r'"
				OUTPUT_VARIABLE CC_INC_PATH)


# kernel common ASM make flags
set(CMAKE_ASM_FLAGS
	"${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} ${UNUSED_MACRO} \
	-fverbose-asm \
	-D__KERNEL__ \
	-D__ASSEMBLY__ -DASM_FILE \
")

# kernel common C make flags
set(CMAKE_C_FLAGS
	"${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
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
	# -Wextra

#===============================================================================================#
#										kernel settings											#
#===============================================================================================#
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

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}

#===============================================================================================#
#										prebuild things											#
#===============================================================================================#

include(./prebuild.cmake)
file(GLOB_RECURSE KBUILD_FILES ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild/*)

#===============================================================================================#
#										compile inputs											#
#===============================================================================================#

file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})
list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})


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

# copy uapi headers to output dir
add_custom_command(TARGET kernel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/include/uapi
                ${CMAKE_BINARY_DIR}/../headers)
install(TARGETS kernel DESTINATION boot)
