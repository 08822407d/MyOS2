#!/usr/bin/env bash

function install_files()
{
	if [ ! -d "/mnt/EFI" ]; then
		mkdir /mnt/EFI
	fi
	if [ ! -d "/mnt/EFI/BOOT" ]; then
		mkdir /mnt/EFI/BOOT
	fi
	cp ../elfbinary_loader.efi /mnt/EFI/BOOT/BOOTX64.EFI
	# cp ./kernel.bin /mnt/kernel.bin
	cp ./kernel /mnt/kernel
	cp ./initd /mnt/initd
	cp ./sh /mnt/sh
	sync
}

if [ "$(uname)" == "Darwin" ]; then
	echo "Working on MAC"

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
	echo "Working on Linux"
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
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
	echo "Working on MinGW"
fi


# qemu-system-x86_64 -bios /home/cheyh/projs/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd -hdb ~/vmware/grub2-test/grub2-test-flat.vmdk -M pc -m 1024 -cpu host -vga virtio -name UEFI -boot order=dc -serial stdio -s -S --enable-kvm -net none