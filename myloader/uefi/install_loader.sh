#!/usr/bin/env bash

sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
cp Build/Shell/DEBUG_GCC5/X64/BootLoader/bootloader/DEBUG/bootloader.efi /mnt/EFI/BOOT/BOOTX64.EFI
sync
sudo umount /mnt