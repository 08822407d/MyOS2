#!/usr/bin/env bash

function install_files()
{
	if [ ! -d "/mnt/EFI" ]; then
		mkdir /mnt/EFI
	fi
	if [ ! -d "/mnt/EFI/BOOT" ]; then
		mkdir /mnt/EFI/BOOT
	fi
	cp ../bootloader.efi /mnt/EFI/BOOT/BOOTX64.EFI
	cp ./kernel.bin /mnt/kernel.bin
	cp ./initd /mnt/initd
	cp ./sh /mnt/sh
	sync
}

if [ "$(uname)" == "Darwin" ]; then
	echo "Working on MAC"
	# hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount ../myos_vdisk-flat.vmdk
	x86_64-elf-objcopy --only-keep-debug kern kernel.debug
	x86_64-elf-objcopy -S -R ".eh_frame" -I elf64-x86-64 -O binary kern kernel.bin
	sudo mount -t msdos /dev/disk2s1 /Users/cheyuho/mount
	cp ./kernel.bin ~/mount/kernel.bin
	sync
	sleep 1
	diskutil unmount ~/mount
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
	echo "Working on Linux"
	# generate kernel debug file and bin
	objcopy --only-keep-debug kernel kernel.debug
	objcopy -S -R ".eh_frame" -I elf64-x86-64 -O binary kernel kernel.bin
	# generate init debug file
	objcopy --only-keep-debug initd initd.debug
	# generate init debug file
	objcopy --only-keep-debug sh sh.debug
	# copy files to virt-disk
	if [ -e "/dev/dm-0" ]; then
		echo "installing on virtual disk"
		sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
		install_files
		sudo umount /mnt
	fi
	# copy files to sata phys-disk
	# if [ -e "/dev/sda1" ]; then
	# 	echo "installing on physical disk"
	# 	sudo mount /dev/sda1 /mnt -o uid=$USER,gid=$USER
	# 	install_files
	# 	sudo umount /mnt
	# fi
	objdump -S kernel > kern_dasm.txt
	objdump -S initd > initd_dasm.txt
	objdump -S sh > sh_dasm.txt
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
	echo "Working on MinGW"
fi


# qemu-system-x86_64 -bios /home/cheyh/projs/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd -hdb ~/vmware/grub2-test/grub2-test-flat.vmdk -M pc -m 1024 -cpu host -vga virtio -name UEFI -boot order=dc -serial stdio -s -S --enable-kvm -net none