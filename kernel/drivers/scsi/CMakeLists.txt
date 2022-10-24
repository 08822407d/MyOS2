file(GLOB SCSI_C_SRCS *.c)
target_sources(kern PRIVATE ${SCSI_C_SRCS})