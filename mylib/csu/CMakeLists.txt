file(GLOB CSU_C_SRCS *.c)
file(GLOB CSU_ASM_SRCS *.S)

add_library(mycrt STATIC)
target_sources(mycrt PRIVATE ${CSU_C_SRCS})
target_sources(mycrt PRIVATE ${CSU_ASM_SRCS})
target_include_directories(mycrt PRIVATE ${PROJECT_SOURCE_DIR}/mylib/include
							${PROJECT_SOURCE_DIR}/mylib/myglibc/include
							${PROJECT_SOURCE_DIR}/mylib/myglibc/sysdeps/stub
							${PROJECT_SOURCE_DIR}/sys)