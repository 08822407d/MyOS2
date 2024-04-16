#!/usr/bin/env bash

function make_install_initranfs() {
	CMAKE_GEN_DIR="$1/build/myinitramfs"
	if [ ! -d $CMAKE_GEN_DIR ]; then
		mkdir $CMAKE_GEN_DIR
	fi
	cd $CMAKE_GEN_DIR
	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
	cmake $1/myinitramfs -DCMAKE_INSTALL_PREFIX=/mnt && make && make install
	sync
	sudo umount /mnt

	objdump -S init > $CMAKE_GEN_DIR/../initd_dasm.txt
	objdump -S sh > $CMAKE_GEN_DIR/../sh_dasm.txt
}
