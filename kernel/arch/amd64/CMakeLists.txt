# machine specific C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
				-DCONFIG_X86_64 -DCONFIG_ZONE_DMA")

# myos tmp machine specific C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
				")

file(GLOB ARCH_C_SRCS *.c)
file(GLOB ARCH_ASM_SRCS *.S)

target_sources(kern PRIVATE ${ARCH_C_SRCS} ${ARCH_ASM_SRCS})

add_subdirectory(kernel)