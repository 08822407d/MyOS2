cmake_minimum_required(VERSION 3.16)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

project(MyOS2 ASM C)

set(PROMPT "==>|")

if (CMAKE_HOST_APPLE)
	message("${PROMPT} Working on MAC : ${CMAKE_SYSTEM_PROCESSOR}\n")
	set (CMAKE_C_COMPILER /opt/local/bin/x86_64-elf-gcc)
	set (CMAKE_ASM_COMPILER /opt/local/bin/x86_64-elf-gcc)
	set (CMAKE_C_LINKER /opt/local/bin/x86_64-elf-gcc)
elseif(CMAKE_HOST_UNIX)
	message("${PROMPT} Working on UNIX : ${CMAKE_SYSTEM_PROCESSOR}\n")
	set (CMAKE_C_COMPILER /usr/bin/gcc)
	set (CMAKE_ASM_COMPILER /usr/bin/gcc)
	set (CMAKE_C_LINKER usr/bin/ld)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}

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

set(CMAKE_ASM_FLAGS
		"${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} ${DEFINATIONS} -m64 \
		-D__ASSEMBLY__ -DASM_FILE")
# message("${PROMPT} ASMFLAGS : ${CMAKE_ASM_FLAGS}")

set(CMAKE_C_FLAGS
		"${CMAKE_C_FLAGS} ${RELEASE_FLAG} ${DEFINATIONS} \
		-m64 -mcmodel=large -fPIC \
		-ffreestanding -nostartfiles \
		-nostdinc -nostdlib \
		-fno-stack-protector \
		-fno-unwind-tables \
		-mabi=sysv \
		-static \
		-fdata-sections -ffunction-sections \
		-ggdb \
		-D__x86_64__ \
		")
		# -v")

execute_process(COMMAND bash -c "gcc --print-file-name=include | tr -d '\n\r'"
				OUTPUT_VARIABLE CC_INC_PATH)

# message("${PROMPT} CC_INC_PATH : ${CC_INC_PATH}")
# message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS}")
# The Compile Options Explaination is in doc/compile_options_explaination.MD

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections")
# message("${PROMPT} LDFLAGS : ${CMAKE_EXE_LINKER_FLAGS}")

#===============================================================================================#
#                                       global files                                            #
#===============================================================================================#
link_directories(${LIBRARY_OUTPUT_PATH})

#===============================================================================================#
#                                       libc.a                                                  #
#===============================================================================================#
add_subdirectory(mylib)

#===============================================================================================#
#                                       kernel.bin                                              #
#===============================================================================================#
add_subdirectory(mykernel)

#===============================================================================================#
#                                       init.bin                                                #
#===============================================================================================#
add_subdirectory(myinitd)
target_link_libraries(initd mylibc mycrt)

#===============================================================================================#
#                                       shell.bin                                               #
#===============================================================================================#
add_subdirectory(myshell)
target_link_libraries(sh mylibc mycrt)