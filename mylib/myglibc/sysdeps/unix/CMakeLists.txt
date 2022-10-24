file(GLOB UNIX_C_SRCS *.c)
file(GLOB UNIX_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${UNIX_C_SRCS} ${UNIX_ASM_SRCS})

add_subdirectory(sysv_linux)