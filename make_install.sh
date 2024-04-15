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
sync
sudo umount /mnt

objdump -S kernel > kern_dasm.txt
objdump -S initd > initd_dasm.txt
objdump -S sh > sh_dasm.txt

bash ../install.sh