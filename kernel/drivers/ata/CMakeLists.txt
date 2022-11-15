file(GLOB ATA_C_SRCS *.c)
target_sources(kern PRIVATE ${ATA_C_SRCS})