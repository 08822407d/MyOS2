set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${CMAKE_CURRENT_SOURCE_DIR}/myshell.lds")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH} -Xlinker -s")

file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
target_include_directories(sh PRIVATE
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include)