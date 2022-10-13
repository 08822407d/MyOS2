target_include_directories(mylibc PUBLIC
				${PROJECT_SOURCE_DIR}/kernel/include
				${PROJECT_SOURCE_DIR}/kernel/arch/${ARCH}/include)

file(GLOB LIBC_SYS_C_SRCS *.c)
file(GLOB LIBC_SYS_ASM_SRCS *.S)
target_sources(mylibc PUBLIC ${LIBC_SYS_C_SRCS} ${LIBC_SYS_ASM_SRCS})