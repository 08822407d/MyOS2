file(GLOB SCHED_C_SRCS *.c)
# file(GLOB SCHED_ASM_SRCS *.S)
target_sources(kern PRIVATE ${SCHED_C_SRCS} ${SCHED_ASM_SRCS})