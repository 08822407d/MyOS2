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
    set(ARCH amd64)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
    set(ARCH aarch64)
endif()

set(DEBUG "-DDEBUG")

#===============================================================================================#
#                                   compile options                                             #
#===============================================================================================#
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${DEBUG} ${DEFINATIONS} -m64")
# message("${PROMPT} ASMFLAGS : ${CMAKE_ASM_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG} ${DEFINATIONS} -mcmodel=large \
    -fno-builtin -fno-stack-protector -Wno-packed-bitfield-compat \
    -nostartfiles -static -ggdb -m64 -fPIC")
# message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none")
# message("${PROMPT} LDFLAGS : ${CMAKE_EXE_LINKER_FLAGS}")

#===============================================================================================#
#                                       global files                                            #
#===============================================================================================#
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/kernel)
link_directories(${LIBRARY_OUTPUT_PATH})
# file(GLOB LIB_C_SRCS ${PROJECT_SOURCE_DIR}/lib/sys/*.c ${PROJECT_SOURCE_DIR}/lib/utils/*.c ${PROJECT_SOURCE_DIR}/lib/string/*.c)
# file(GLOB LIB_ASM_SRCS ${PROJECT_SOURCE_DIR}/lib/sys/*.S ${PROJECT_SOURCE_DIR}/lib/utils/*.S ${PROJECT_SOURCE_DIR}/lib/string/*.S)
# file(GLOB USER_LIB_S_SRCS ${PROJECT_SOURCE_DIR}/lib/user_only/*.c)

#===============================================================================================#
#                                       libc.a                                                  #
#===============================================================================================#
add_subdirectory(lib)

#===============================================================================================#
#                                       kernel.bin                                              #
#===============================================================================================#
add_subdirectory(kernel)
target_link_libraries(kernel mylibc)

#===============================================================================================#
#                                       init.bin                                                #
#===============================================================================================#
add_subdirectory(initd)
target_link_libraries(init mylibc)
