#!/usr/bin/env bash

if [ ! -d "./build" ]; then
	mkdir ./build
fi

rm -rf ./build/*
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/mnt
make
sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
if [ ! -d "/mnt/boot" ]; then
	mkdir /mnt/boot
fi
make install
if [ ! -d "/mnt/EFI" ]; then
	mkdir /mnt/EFI
fi
if [ ! -d "/mnt/EFI/BOOT" ]; then
	mkdir /mnt/EFI/BOOT
fi
cp ../elf_loader.efi /mnt/EFI/BOOT/BOOTX64.EFI
sync
sudo umount /mnt

objdump -S kernel > kern_dasm.txt
objdump -S init > initd_dasm.txt
objdump -S sh > sh_dasm.txt

# bash ../install.sh