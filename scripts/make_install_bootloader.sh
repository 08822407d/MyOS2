#!/usr/bin/env bash

function make_install_bootloader() {
	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
	cmake $1/myloader -DCMAKE_INSTALL_PREFIX=/mnt && make && make install
	sync
	sudo umount /mnt
}
