#!/usr/bin/env bash

qemu-system-x86_64 -s -S -bios ~/vmware/grub2-test/OVMF.fd -hdb ~/vmware/grub2-test/grub2-test-flat.vmdk -M pc -m 2048 -smp threads=1,cores=1,sockets=1 -vga virtio -name UEFI -boot order=dc -net none -nographic