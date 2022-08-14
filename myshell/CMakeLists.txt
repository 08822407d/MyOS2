set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${PROJECT_SOURCE_DIR}/myshell/shell.lds")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CC_INC_PATH}")


file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(shell ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
target_include_directories(shell PRIVATE
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86)