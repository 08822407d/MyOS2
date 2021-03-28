#!/usr/bin/env bash

if [ "$(uname)" == "Darwin" ]; then
    echo "Working on MAC"
    # hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount myos_vdisk-flat.vmdk
    x86_64-elf-objcopy --only-keep-debug system kernel.debug
    x86_64-elf-objcopy -S -R ".eh_frame" -R ".comment" -R ".note.gnu.build-id" -I elf64-x86-64 -O binary system kernel.bin
    sudo mount -t msdos /dev/disk2s1 /Users/cheyuho/mount
    cp ./kernel.bin ~/mount/kernel.bin
    sync
    sleep 1
    diskutil unmount ~/mount
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    echo "Working on Linux"
    objcopy --only-keep-debug system kernel.debug
    objcopy -S -R ".eh_frame" -R ".comment" -R ".note.gnu.build-id" -I elf64-x86-64 -O binary system kernel.bin
    sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
    cp ./kernel.bin /mnt/kernel.bin
    sync
    sleep 1
    sudo umount /mnt
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    echo "Working on MinGW"
fi

# qemu-system-x86_64 -bios /home/cheyh/projs/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd -hdb ~/vmware/grub2-test/grub2-test-flat.vmdk -M pc -m 1024 -cpu host -vga virtio -name UEFI -boot order=dc -serial stdio -s -S --enable-kvm -net none