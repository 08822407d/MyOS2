file(GLOB CSU_C_SRCS *.c)
file(GLOB CSU_ASM_SRCS *.S)

add_library(mycrt STATIC)
target_sources(mycrt PRIVATE ${CSU_C_SRCS})
target_sources(mycrt PRIVATE ${CSU_ASM_SRCS})