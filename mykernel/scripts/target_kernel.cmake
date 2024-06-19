target_sources(kernel PRIVATE ${KERNEL_C_SRCS} ${KERNEL_ASM_SRCS})
target_include_directories(kernel PRIVATE
	${INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/../myloader
)
target_link_options(kernel PRIVATE
    -T ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kernel.lds
)

add_subdirectory(arch)
add_subdirectory(init)
add_subdirectory(sched)
add_subdirectory(mm)
add_subdirectory(fs)
add_subdirectory(time)
add_subdirectory(device)
add_subdirectory(drivers)
add_subdirectory(block)
add_subdirectory(lib)
add_subdirectory(printk)

add_subdirectory(klib)
add_subdirectory(debug)
add_subdirectory(lock_IPC)
add_subdirectory(kactive)

# copy uapi headers to output dir
add_custom_command(TARGET kernel POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${PROJECT_SOURCE_DIR}/include/uapi
			${CMAKE_BINARY_DIR}/../headers)
install(TARGETS kernel DESTINATION boot)