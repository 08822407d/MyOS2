add_library(mylibc STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CC_INC_PATH}")
target_include_directories(mylibc PUBLIC
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sys
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/unix
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/generic
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/stub)

add_subdirectory(sys)
add_subdirectory(errno)
add_subdirectory(stdio)
add_subdirectory(stdlib)
add_subdirectory(string)
add_subdirectory(exit)

add_subdirectory(posix)
add_subdirectory(sysdeps)
