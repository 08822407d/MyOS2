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
	cp ./init.bin /mnt/init.bin
	cp ./initd /mnt/initd
	cp ./shell.bin /mnt/shell.bin
	cp ./shell /mnt/shell
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
	objcopy --only-keep-debug kernelexec kernel.debug
	objcopy -S -R ".eh_frame" -I elf64-x86-64 -O binary kernelexec kernel.bin
	# generate init debug file and bin
	objcopy --only-keep-debug initdexec initd.debug
	objcopy -S -R ".eh_frame" -I elf64-x86-64 -O binary initdexec init.bin
	objcopy -S -R ".eh_frame" -I elf64-x86-64 -O elf64-x86-64 initdexec initd
	# generate init debug file and bin
	objcopy --only-keep-debug shellexec shell.debug
	objcopy -S -R ".eh_frame" -I elf64-x86-64 -O binary shellexec shell.bin
	objcopy -S -R ".eh_frame" -I elf64-x86-64 -O elf64-x86-64 shellexec shell
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
	objdump -S kernelexec > kern_dasm.txt
	objdump -S initdexec > initd_dasm.txt
	objdump -S shellexec > sh_dasm.txt
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
	echo "Working on MinGW"
fi


# qemu-system-x86_64 -bios /home/cheyh/projs/edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd -hdb ~/vmware/grub2-test/grub2-test-flat.vmdk -M pc -m 1024 -cpu host -vga virtio -name UEFI -boot order=dc -serial stdio -s -S --enable-kvm -net none