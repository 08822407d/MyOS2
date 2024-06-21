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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UNUSED_MACRO} \
	-D__KERNEL__ \
	-DCONFIG_FLATMEM -DCONFIG_NR_CPUS=256 \
	-DCONFIG_64BIT -DCONFIG_PHYS_ADDR_T_64BIT \
	-DCONFIG_ZONE_DMA -DCONFIG_ZONE_DMA32 \
	-DCONFIG_SLUB \
	-DCONFIG_ARCH_HAS_SYSCALL_WRAPPER \
	-DCONFIG_BUG \
	-DGRUB2_BOOTUP_SUPPORT \
")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
# 	-DCONFIG_INTEL_X64_GDT_LAYOUT \
# ")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}

set(INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/linux/kernel
	${PROJECT_SOURCE_DIR}/arch/${ARCH}
	${PROJECT_SOURCE_DIR}/arch/${ARCH}/include
)