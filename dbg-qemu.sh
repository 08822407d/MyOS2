#!/usr/bin/env bash


ZBOOK="$HOME/vmware/grub2-test/grub2-test.vmx"
HOME_ASUS="$HOME/vmware/myos2test/myos2test.vmx"

if [ $1 == "zbook" ]; then
	qemu-system-x86_64 -s -S -bios ./OVMF.fd -hdb $ZBOOK -M pc -m 8192 -smp threads=1,cores=1,sockets=1 -vga virtio -name UEFI -boot order=dc -net none -nographic
elif [ $1 == "asus" ]; then
	qemu-system-x86_64 -s -S -bios ./OVMF.fd -hdb $HOME_ASUS -M pc -m 8192 -smp threads=1,cores=1,sockets=1 -vga virtio -name UEFI -boot order=dc -net none -nographic
fi
