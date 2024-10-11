#!/usr/bin/env bash

function make_install_initranfs() {
	CMAKE_GEN_DIR="$1/build/myinitramfs"
	if [ ! -d $CMAKE_GEN_DIR ]; then
		mkdir $CMAKE_GEN_DIR
	fi
	cd $CMAKE_GEN_DIR

	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
	rm /mnt/boot/init
	rm /mnt/boot/sh
	cmake $1/myinitramfs $2 -DLINK_TYPE=$3 -DCMAKE_INSTALL_PREFIX=/mnt && make && make install
	sync
	sudo umount /mnt

	objdump -S init > $CMAKE_GEN_DIR/../initd_dasm.txt
	objdump -S sh > $CMAKE_GEN_DIR/../sh_dasm.txt

	cp init $CMAKE_GEN_DIR/../initd
	cp sh $CMAKE_GEN_DIR/../sh
}
