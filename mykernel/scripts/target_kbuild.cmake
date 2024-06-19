#===============================================================================================#
#									asm-offsets things											#
#===============================================================================================#
set(KBUILD_DIR ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild)
# message("${PROMPT} KBUILD_DIR : ${KBUILD_DIR} ${PROMPT_END}")

# paths and options/flags to generate asm-offsets.c
set(ASM_OFFSET_C_SRCS ${KBUILD_DIR}/asm-offsets.c)
set(ASM_OFFSET_ASM_DST ${KBUILD_DIR}/asm-offsets.s)

# paths to generate asm-offsets.h
set(ASM_OFFSET_COPY_TEMP_SCRIPT ${PROJECT_SOURCE_DIR}/scripts/asm-offset_copy_asm.py)
set(ASM_OFFSET_GEN_SCRIPT ${PROJECT_SOURCE_DIR}/scripts/asm-offsets_generate.py)
set(ASM_OFFSET_HEADER_DST ${PROJECT_SOURCE_DIR}/include/generated/asm-offsets.h)

# message("${PROMPT} ASM_OFFSET_C_SRCS : ${ASM_OFFSET_C_SRCS} ${PROMPT_END}")
add_executable(asm-offset)
target_sources(asm-offset PRIVATE ${ASM_OFFSET_C_SRCS})
target_compile_options(asm-offset PRIVATE
    -save-temps
)
target_include_directories(asm-offset PRIVATE
	${INCLUDE_DIRS}
)
add_custom_command(
    TARGET asm-offset POST_BUILD
    # COMMAND echo "kbuild call test"
    COMMAND rm -f ${ASM_OFFSET_ASM_DST} && rm -f ${ASM_OFFSET_HEADER_DST}
    COMMAND python3 ${ASM_OFFSET_COPY_TEMP_SCRIPT} "--input_dir=${CMAKE_CURRENT_BINARY_DIR}" "--output=${ASM_OFFSET_ASM_DST}"
    COMMAND python3 ${ASM_OFFSET_GEN_SCRIPT} "--input=${ASM_OFFSET_ASM_DST}" "--output=${ASM_OFFSET_HEADER_DST}"
)
add_dependencies(kernel asm-offset)