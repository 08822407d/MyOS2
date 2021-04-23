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
    # set (CMAKE_C_COMPILER /usr/bin/gcc)
    # set (CMAKE_ASM_COMPILER /usr/bin/as)
    # set (CMAKE_C_LINKER usr/bin/ld)
endif()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(ARCH amd64)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
    set(ARCH aarch64)
endif()

set(DEFINATIONS "-DDEBUG -DUSE_APIC")

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${DEFINATIONS} -m64")
message("${PROMPT} ASMFLAGS : ${CMAKE_ASM_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEFINATIONS} -mcmodel=large \
                    -fno-builtin -fno-stack-protector -Wno-packed-bitfield-compat \
                    -nostartfiles -static -no-pie -ggdb -m64 -fPIC \
                    -T ${PROJECT_SOURCE_DIR}/kernel/arch/${ARCH}/kernel.lds")
message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--build-id=none")
message("${PROMPT} LDFLAGS : ${CMAKE_EXE_LINKER_FLAGS}")

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB KERNEL_C_SRCS kernel/*.c)
file(GLOB KERNEL_ASM_SRCS kernel/*.S)

file(GLOB K_ARCH_C_SRCS kernel/arch/${ARCH}/*.c)
file(GLOB K_ARCH_ASM_SRCS kernel/arch/${ARCH}/*.S)

file(GLOB K_LIB_C_SRCS kernel/klib/*.c)

file(GLOB LIB_C_SRCS lib/*.c lib/utils/*.c lib/string/*.c)
file(GLOB LIB_ASM_SRCS lib/*.S lib/utils/*.S lib/string/*.S)

add_executable(system ${K_ARCH_ASM_SRCS} ${K_ARCH_C_SRCS} ${KERNEL_ASM_SRCS} ${K_LIB_C_SRCS} ${KERNEL_C_SRCS}
                ${LIB_C_SRCS} ${LIB_ASM_SRCS})

# add_custom_command(OUTPUT kernel.debug
#                     COMMAND objcopy --only-keep-debug system kernel.debug
#                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR})