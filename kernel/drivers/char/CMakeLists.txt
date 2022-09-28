file(GLOB DEVCHAR_C_SRCS *.c)
target_sources(kern PRIVATE ${DEVCHAR_C_SRCS})