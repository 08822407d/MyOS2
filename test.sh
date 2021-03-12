#!/bin/bash

objcopy --only-keep-debug system kernel.debug

objcopy -S -R ".eh_frame" -R ".comment" -R ".note.gnu.build-id" -I elf64-x86-64 -O binary system kernel.bin

sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER

cp ./kernel.bin /mnt/kernel.bin

sync

sudo umount /mnt

# qemu-system-x86_64 -bios /home/cheyh/projs/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd -hdb ~/vmware/grub2-test/grub2-test-flat.vmdk -M pc -m 1024 -cpu qemu64 -vga cirrus -name UEFI -boot order=dc -serial stdio -s -S --enable-kvm -net none