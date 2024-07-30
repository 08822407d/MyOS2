file(GLOB INIT_C_SRCS ./*.c)
file(GLOB INIT_ASM_SRCS ./*.S)

add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})
target_link_options(init PRIVATE
    -Wl,-Ttext-segment=0x2400000
)