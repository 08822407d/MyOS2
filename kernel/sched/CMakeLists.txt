file(GLOB SCHED_C_SRCS *.c)
target_sources(kern PRIVATE ${SCHED_C_SRCS})