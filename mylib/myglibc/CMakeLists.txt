add_library(mylibc STATIC)
target_include_directories(mylibc PRIVATE ${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/mylib/include
                    ${PROJECT_SOURCE_DIR}/kernel 
                    ${PROJECT_SOURCE_DIR}/sys)

add_subdirectory(sys)
add_subdirectory(errno)
add_subdirectory(stdio)
add_subdirectory(stdlib)
add_subdirectory(string)
add_subdirectory(exit)
add_subdirectory(unix)
