# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${CMAKE_CURRENT_SOURCE_DIR}/myshell.lds")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH} -Xlinker -s")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")

file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
target_include_directories(sh PRIVATE
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc/include)