add_library(mylibc STATIC)

add_subdirectory(gen)
add_subdirectory(sys)
add_subdirectory(stdio)
add_subdirectory(stdlib)
add_subdirectory(string)