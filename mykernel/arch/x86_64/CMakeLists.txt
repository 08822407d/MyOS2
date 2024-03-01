# machine specific C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
				-DCONFIG_X86_64 -DCONFIG_ZONE_DMA")

# myos tmp machine specific C make flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
				")

add_subdirectory(kernel)
add_subdirectory(entry)
add_subdirectory(lib)
add_subdirectory(pci)


add_subdirectory(smp)
add_subdirectory(asm)
add_subdirectory(myos)