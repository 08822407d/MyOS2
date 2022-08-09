add_library(mylibc STATIC)
target_include_directories(mylibc PRIVATE ${PROJECT_SOURCE_DIR}/mylib/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/include
                    ${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/stub
                    ${PROJECT_SOURCE_DIR}/sys)

add_subdirectory(sys)
add_subdirectory(errno)
add_subdirectory(stdio)
add_subdirectory(stdlib)
add_subdirectory(string)
add_subdirectory(exit)

add_subdirectory(posix)
add_subdirectory(sysdeps)
