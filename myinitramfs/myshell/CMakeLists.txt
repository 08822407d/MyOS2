file(GLOB SHELL_C_SRCS ./*.c)
file(GLOB SHELL_ASM_SRCS ./*.S)

add_executable(sh ${SHELL_C_SRCS} ${SHELL_ASM_SRCS})
target_link_options(sh PRIVATE
    -Wl,-Ttext-segment=0x2800000
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
target_include_directories(sh PRIVATE
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc/include)