cmake_minimum_required(VERSION 3.16)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
project(MyOS2_Kernel ASM C)

include(./options_flags.cmake)

#===============================================================================================#
#										prebuild things											#
#===============================================================================================#

file(GLOB_RECURSE KBUILD_FILES ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild/*)

#===============================================================================================#
#										compile inputs											#
#===============================================================================================#

file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})
list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})

add_executable(kernel)
target_sources(kernel PRIVATE ${KERNEL_C_SRCS} ${KERNEL_ASM_SRCS})
target_include_directories(kernel PRIVATE
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/linux/kernel
	${PROJECT_SOURCE_DIR}/arch/${ARCH}
	${PROJECT_SOURCE_DIR}/arch/${ARCH}/include
	${PROJECT_SOURCE_DIR}/../myloader
)

include(./kbuild.cmake)

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
