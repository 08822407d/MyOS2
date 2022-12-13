file(GLOB PROC_C_SRCS *.c)
target_sources(kern PRIVATE ${PROC_C_SRCS})