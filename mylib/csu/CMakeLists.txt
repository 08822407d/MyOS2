file(GLOB CSU_C_SRCS *.c)
file(GLOB CSU_ASM_SRCS *.S)

add_library(mycrt STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CC_INC_PATH}")
target_sources(mycrt PRIVATE ${CSU_C_SRCS})
target_sources(mycrt PRIVATE ${CSU_ASM_SRCS})
target_include_directories(mycrt PRIVATE
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86)