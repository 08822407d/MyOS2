cmake_minimum_required(VERSION 3.16)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
project(MyOS2_BootLoader ASM C)

install(PROGRAMS elf_loader.efi DESTINATION EFI/BOOT)
