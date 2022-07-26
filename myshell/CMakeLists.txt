set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${PROJECT_SOURCE_DIR}/myshell/shell.lds")


file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(shell ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
target_include_directories(shell PRIVATE ${PROJECT_SOURCE_DIR}/include
							${PROJECT_SOURCE_DIR}/mylib/include
							${PROJECT_SOURCE_DIR}/sys)