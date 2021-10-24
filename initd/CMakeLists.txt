set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${PROJECT_SOURCE_DIR}/initd/init.lds")

file(GLOB INIT_C_SRCS ./*.c)
file(GLOB INIT_ASM_SRCS ./*.S)

add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})