cmake_minimum_required(VERSION 3.16)

add_library(mylibc STATIC)

add_subdirectory(stdio)
add_subdirectory(stdlib)
add_subdirectory(string)