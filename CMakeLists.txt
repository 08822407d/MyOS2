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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG} ${DEFINATIONS} \
    -m64 -mcmodel=large -fPIC \
    -ffreestanding -nostartfiles \
    -nostdinc -nostdlib \
    -fno-stack-protector \
    -fno-unwind-tables \
    -mabi=sysv -mno-red-zone \
    -static \
    -fdata-sections -ffunction-sections \
    -ggdb")
# message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS}")
# The Compile Options Explaination is in doc/compile_options_explaination.MD

# Compile Options Explainations:

# -mcmodel=large
#   Generate code for the large model.
#   This model makes no assumptions about addresses and sizes of sections.

# -ffreestanding
#   Assert that compilation targets a freestanding environment. This implies -fno-builtin.
#   A freestanding environment is one in which the standard library may not exist,
#   and program startup may not necessarily be at main.
#   The most obvious example is an OS kernel. This is equivalent to -fno-hosted.

# -nostdinc
#   Do not search the standard system directories for header files.
#   Only the directories explicitly specified with -I, -iquote, -isystem,
#   and/or -idirafter options (and the directory of the current file, if appropriate) are searched.

# -nostdinc++
#   Do not search for header files in the C++-specific standard directories,
#   but do still search the other standard directories. (This option is used when building the C++ library.)

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
add_subdirectory(kernel)

#===============================================================================================#
#                                       init.bin                                                #
#===============================================================================================#
add_subdirectory(initd)
target_link_libraries(init mylibc mycrt)

#===============================================================================================#
#                                       shell.bin                                               #
#===============================================================================================#
add_subdirectory(myshell)
target_link_libraries(shell mylibc mycrt)