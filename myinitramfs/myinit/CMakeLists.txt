# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${CMAKE_CURRENT_SOURCE_DIR}/init.lds")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH} -Xlinker -s")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")

file(GLOB INIT_C_SRCS ./*.c)
file(GLOB INIT_ASM_SRCS ./*.S)

add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})
target_include_directories(init PUBLIC
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/myinitramfs/mylib/myglibc/include)