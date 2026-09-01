# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - migration helper; adopt target by target
# open_questions: []
# ---

include_guard(GLOBAL)

function(myos2_require_program out_var program)
  find_program(_myos2_program NAMES "${program}")
  if(NOT _myos2_program)
    message(FATAL_ERROR "Required program not found: ${program}")
  endif()
  set("${out_var}" "${_myos2_program}" PARENT_SCOPE)
endfunction()

function(myos2_compiler_builtin_include out_var)
  execute_process(
    COMMAND "${CMAKE_C_COMPILER}" -print-file-name=include
    OUTPUT_VARIABLE _include
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY
  )
  if(NOT IS_DIRECTORY "${_include}")
    message(FATAL_ERROR "Compiler builtin include directory not found: ${_include}")
  endif()
  set("${out_var}" "${_include}" PARENT_SCOPE)
endfunction()

function(myos2_add_kernel_options target)
  target_compile_definitions("${target}" INTERFACE
    __KERNEL__
    __x86_64__
    CONFIG_FLATMEM
    CONFIG_NR_CPUS=256
    CONFIG_64BIT
    CONFIG_PHYS_ADDR_T_64BIT
    CONFIG_ZONE_DMA
    CONFIG_ZONE_DMA32
    CONFIG_SLUB
    CONFIG_ARCH_HAS_SYSCALL_WRAPPER
    CONFIG_BUG
    GRUB2_BOOTUP_SUPPORT
    CONFIG_HYPERVISOR_GUEST
    CONFIG_KVM_GUEST
    $<$<CONFIG:Debug>:DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:RELEASE>
  )
  target_compile_options("${target}" INTERFACE
    $<$<COMPILE_LANGUAGE:C>:
      -m64;-mcmodel=kernel;-fno-pie;-fno-pic;-ffreestanding;
      -nostartfiles;-nostdinc;-nostdlib;-fno-stack-protector;
      -fno-unwind-tables;-fdata-sections;-ffunction-sections;
      -ggdb;-mabi=sysv;-mno-red-zone
    >
    $<$<COMPILE_LANGUAGE:ASM>:
      -m64;-mcmodel=kernel;-fverbose-asm;-fno-pie;-fno-pic;
      -D__ASSEMBLY__;-DASM_FILE
    >
  )
  target_link_options("${target}" INTERFACE
    -nostdlib
    -static
    -Wl,--build-id=none,--gc-sections
  )
endfunction()

function(myos2_collect_sources out_var root)
  file(GLOB_RECURSE _sources CONFIGURE_DEPENDS
    "${root}/*.c"
    "${root}/*.S"
  )
  list(FILTER _sources EXCLUDE REGEX "/(build|out|generated|tests?)/")
  set("${out_var}" "${_sources}" PARENT_SCOPE)
endfunction()
