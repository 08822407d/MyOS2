# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - set MYOS2_MUSL_CC or CMAKE_C_COMPILER before the first configure
# open_questions:
#   - mylib remains intentionally disabled until its ownership is decided
# ---

cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_C_COMPILER AND DEFINED ENV{MYOS2_MUSL_CC})
  set(CMAKE_C_COMPILER "$ENV{MYOS2_MUSL_CC}" CACHE FILEPATH
      "musl compiler for MyOS2 initramfs")
endif()

if(NOT DEFINED CMAKE_C_COMPILER AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message(FATAL_ERROR
    "Configure with -DCMAKE_C_COMPILER=/path/to/musl-gcc, "
    "-DCMAKE_TOOLCHAIN_FILE=..., or set MYOS2_MUSL_CC")
endif()

project(MyOS2_Initramfs LANGUAGES ASM C)

set(MYOS2_INITRAMFS_LINK_TYPE "dynamic" CACHE STRING "dynamic or static")
set_property(CACHE MYOS2_INITRAMFS_LINK_TYPE PROPERTY STRINGS dynamic static)

add_subdirectory(myinit)
add_subdirectory(myshell)

foreach(_target IN ITEMS init sh)
  target_compile_options("${_target}" PRIVATE -m64 -ggdb)
  target_compile_definitions("${_target}" PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:RELEASE>
  )
  if(MYOS2_INITRAMFS_LINK_TYPE STREQUAL "static")
    target_link_options("${_target}" PRIVATE -static)
  elseif(NOT MYOS2_INITRAMFS_LINK_TYPE STREQUAL "dynamic")
    message(FATAL_ERROR "MYOS2_INITRAMFS_LINK_TYPE must be dynamic or static")
  endif()
endforeach()

install(TARGETS init sh DESTINATION boot)
