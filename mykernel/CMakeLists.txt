cmake_minimum_required(VERSION 3.16)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
project(MyOS2_Kernel ASM C)


#===============================================================================================#
#										general things											#
#===============================================================================================#

set(SCRIPTS_PATH ${PROJECT_SOURCE_DIR}/scripts)
include(${SCRIPTS_PATH}/options_flags.cmake)

add_executable(kernel)


#===============================================================================================#
#										prebuild things											#
#===============================================================================================#

include(${SCRIPTS_PATH}/target_kbuild.cmake)

file(GLOB_RECURSE KBUILD_FILES ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild/*)
# message("${PROMPT} KBUILD_FILES : ${KBUILD_FILES} ${PROMPT_END}")


#===============================================================================================#
#										compile inputs											#
#===============================================================================================#

file(GLOB_RECURSE KERNEL_C_SRCS ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE KERNEL_ASM_SRCS ${PROJECT_SOURCE_DIR}/*.S)
list(REMOVE_ITEM KERNEL_C_SRCS ${KBUILD_FILES})
list(REMOVE_ITEM KERNEL_ASM_SRCS ${KBUILD_FILES})

include(${SCRIPTS_PATH}/target_kernel.cmake)

