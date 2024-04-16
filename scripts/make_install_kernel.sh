#!/usr/bin/env bash

function make_install_kernel() {
	CMAKE_GEN_DIR="$1/build/mykernel"
	if [ ! -d $1CMAKE_GEN_DIR ]; then
		mkdir $CMAKE_GEN_DIR
	fi
	cd $CMAKE_GEN_DIR
	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
	cmake $1/mykernel -DCMAKE_INSTALL_PREFIX=/mnt && make && make install
	sync
	sudo umount /mnt

	objdump -S kernel > $CMAKE_GEN_DIR/../kern_dasm.txt
}
