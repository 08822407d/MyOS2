# message("${PROMPT} CFLAGS : ${CMAKE_C_FLAGS} ${PROMPT_END}")

set(INCLUDE_PATHS "\
    -I ${PROJECT_SOURCE_DIR} \
    -I ${PROJECT_SOURCE_DIR}/include \
    -I ${PROJECT_SOURCE_DIR}/include/linux/kernel \
    -I ${PROJECT_SOURCE_DIR}/arch/${ARCH} \
    -I ${PROJECT_SOURCE_DIR}/arch/${ARCH}/include \
")
set(KBUILD_DIR ${PROJECT_SOURCE_DIR}/arch/${ARCH}/kbuild)
# message("${PROMPT} KBUILD_DIR : ${KBUILD_DIR} ${PROMPT_END}")

# paths and options/flags to generate asm-offsets.c
file(GLOB_RECURSE ASM_OFFSET_C_SRCS ${KBUILD_DIR}/asm-offsets.c)
set(ASM_OFFSET_ASM_DST ${KBUILD_DIR}/asm-offsets.s)
set(GEN_ASM_OFFSET_CMD_ARGS "${INCLUDE_PATHS} ${CMAKE_C_FLAGS} \
     ${INCLUDE_FLAGS} -S ${ASM_OFFSET_C_SRCS} -o ${ASM_OFFSET_ASM_DST}")
set(GEN_ASM_OFFSET_CMD_ARGS_RSP ${CMAKE_BINARY_DIR}/gen_asm_offsets.rsp)
file(WRITE ${GEN_ASM_OFFSET_CMD_ARGS_RSP} ${GEN_ASM_OFFSET_CMD_ARGS})

# paths to generate asm-offsets.h
set(ASM_OFFSET_GEN_SCRIPT ${PROJECT_SOURCE_DIR}/scripts/asm-offsets_generate.py)
set(ASM_OFFSET_HEADER_DST ${PROJECT_SOURCE_DIR}/include/generated/asm-offsets.h)

message("${PROMPT} ASM_OFFSET_C_SRCS : ${ASM_OFFSET_C_SRCS} ${PROMPT_END}")
add_custom_target(asm-offset ALL)  
add_custom_command(
    TARGET asm-offset
    PRE_BUILD     
    COMMAND ${CMAKE_C_COMPILER} @${GEN_ASM_OFFSET_CMD_ARGS_RSP} &&
            python3 ${ASM_OFFSET_GEN_SCRIPT} "--input=${ASM_OFFSET_ASM_DST}" "--output=${ASM_OFFSET_HEADER_DST}"
    # COMMAND python3 ${ASM_OFFSET_GEN_SCRIPT} "--input=${ASM_OFFSET_ASM_DST}" "--output=${ASM_OFFSET_HEADER_DST}"
)