cmake_minimum_required(VERSION 3.16)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
project(MyOS2_Initramfs ASM C)


set(PROMPT "======>>>")
set(PROMPT_END "<<<======")

#===============================================================================================#
#                                   global options                                              #
#===============================================================================================#
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(ARCH x86_64)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
	set(ARCH aarch64)
endif()


#===============================================================================================#
#                                   compile options                                             #
#===============================================================================================#
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(RELEASE_FLAG "-DDEBUG")
	message("${PROMPT} DEBUG ${PROMPT_END}")
elseif ()
	message("${PROMPT} RELEASE")
	set(RELEASE_FLAG "-DRELEASE")
	set(UNUSED_MACRO "-Wunused-macros")
endif()

execute_process(COMMAND bash -c "gcc --print-file-name=include | tr -d '\n\r'"
				OUTPUT_VARIABLE CC_INC_PATH)


set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${RELEASE_FLAG} \
	-m64 \
")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE_FLAG} \
	-m64 -fPIC \
	-static \
	-ggdb \
")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none,--gc-sections")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}) # Force generate binaries to ${CMAKE_BINARY_DIR}
link_directories(${LIBRARY_OUTPUT_PATH})


add_subdirectory(mylib)
add_subdirectory(myinit)
add_subdirectory(myshell)

install(TARGETS init DESTINATION boot)
install(TARGETS sh DESTINATION boot)