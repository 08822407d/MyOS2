add_library(mylibc STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/string
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/misc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/x86
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/gnu
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix/sysv_linux/x86)

add_subdirectory(stdio)
add_subdirectory(malloc)
add_subdirectory(string)
add_subdirectory(exit)

add_subdirectory(syscall)
add_subdirectory(posix)
add_subdirectory(sysdeps)
add_subdirectory(misc)
add_subdirectory(time)
add_subdirectory(io)
add_subdirectory(libio)
