# ---
# task_id: MYOS2-DR-001
# status: proposal
# base_snapshot:
#   branch: time
#   commit: a039d9803ade94c67918930525530d2a1b46e9f0
# warnings:
#   - consumes a selected EFI artifact; does not hide an external EDK2 build
# open_questions:
#   - authoritative EFI build pipeline remains undecided
# ---

cmake_minimum_required(VERSION 3.16)
project(MyOS2_BootLoader LANGUAGES NONE)

set(MYOS2_BOOTLOADER_EFI "" CACHE FILEPATH "Path to the built BOOTX64.EFI")
if(NOT MYOS2_BOOTLOADER_EFI AND DEFINED ENV{MYOS2_BOOTLOADER_EFI})
  set(MYOS2_BOOTLOADER_EFI "$ENV{MYOS2_BOOTLOADER_EFI}")
endif()

if(NOT MYOS2_BOOTLOADER_EFI AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BOOTX64.EFI")
  set(MYOS2_BOOTLOADER_EFI "${CMAKE_CURRENT_SOURCE_DIR}/BOOTX64.EFI")
  message(WARNING
    "MYOS2_BOOTLOADER_EFI unset; using repository prebuilt: "
    "${MYOS2_BOOTLOADER_EFI}")
endif()

if(NOT EXISTS "${MYOS2_BOOTLOADER_EFI}")
  message(FATAL_ERROR "Set MYOS2_BOOTLOADER_EFI to a built EFI executable")
endif()

message(STATUS "Installing EFI artifact: ${MYOS2_BOOTLOADER_EFI}")
install(PROGRAMS "${MYOS2_BOOTLOADER_EFI}"
        DESTINATION EFI/BOOT
        RENAME BOOTX64.EFI)
