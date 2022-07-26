set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -T ${PROJECT_SOURCE_DIR}/initd/initd.lds")


file(GLOB INIT_C_SRCS ./*.c)
file(GLOB INIT_ASM_SRCS ./*.S)

add_executable(init ${INIT_C_SRCS} ${INIT_ASM_SRCS})
target_include_directories(init PRIVATE ${PROJECT_SOURCE_DIR}/include
							${PROJECT_SOURCE_DIR}/mylib/include
							${PROJECT_SOURCE_DIR}/sys)