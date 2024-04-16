add_library(mylibc STATIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__USE_GNU -D_GNU_SOURCE -I${CC_INC_PATH}")
target_include_directories(mylibc PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/sys
                    ${CMAKE_CURRENT_SOURCE_DIR}/string
                    ${CMAKE_CURRENT_SOURCE_DIR}/misc
                    ${CMAKE_CURRENT_SOURCE_DIR}/sysdeps/x86
                    ${CMAKE_CURRENT_SOURCE_DIR}/sysdeps/generic
                    ${CMAKE_CURRENT_SOURCE_DIR}/sysdeps/gnu
                    ${CMAKE_CURRENT_SOURCE_DIR}/sysdeps/unix/sysv_linux
                    ${CMAKE_CURRENT_SOURCE_DIR}/sysdeps/unix/sysv_linux/x86)

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
