add_library(mylibc STATIC)

add_subdirectory(sys)
add_subdirectory(errno)
add_subdirectory(stdio)
add_subdirectory(stdlib)
add_subdirectory(string)
add_subdirectory(exit)
add_subdirectory(dirent)
