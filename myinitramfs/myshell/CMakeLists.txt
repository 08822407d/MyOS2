file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})