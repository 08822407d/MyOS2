#!/usr/bin/env bash

function make_install_bootloader() {
	CMAKE_GEN_DIR="$1/build/bootloader"
	if [ ! -d $CMAKE_GEN_DIR ]; then
		mkdir $CMAKE_GEN_DIR
	fi
	cd $CMAKE_GEN_DIR

	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
	cmake $1/myloader -DCMAKE_INSTALL_PREFIX=/mnt && make && make install
	sync
	sudo umount /mnt
}
