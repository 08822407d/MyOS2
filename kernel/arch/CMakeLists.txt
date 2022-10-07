add_subdirectory(${ARCH})

target_include_directories(kern PRIVATE ${PROJECT_SOURCE_DIR}/kernel/arch/amd64/include)