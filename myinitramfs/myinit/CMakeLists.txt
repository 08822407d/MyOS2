file(GLOB INIT_C_SRCS ./*.c)
file(GLOB INIT_ASM_SRCS ./*.S)

add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})
target_link_options(init PRIVATE
    -Wl,-Ttext-segment=0x2400000
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
target_include_directories(init PUBLIC
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc/include)