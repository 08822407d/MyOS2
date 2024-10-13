#!/usr/bin/env bash

LIB_PATH="/usr/lib/x86_64-linux-gnu"
MYOS2_LIB_PATH="/mnt$LIB_PATH"
MUSL_LIB_PATH="/home/cheyh/projs/musl/build/lib"
MUSL_LD_PATH="/home/cheyh/projs/musl/build/sys"
MUSL_DYN_LD="$LIB_PATH/ld-musl-x86_64.so.1"

function prepare_libc_ld() {
	if [ ! -d $MYOS2_LIB_PATH ]; then
		mkdir -p $MYOS2_LIB_PATH
	fi
	cp $MUSL_LIB_PATH/libc.so $MYOS2_LIB_PATH
	cp $MUSL_LD_PATH/ld-musl-x86_64.so.1 $MYOS2_LIB_PATH
}


function make_install_initranfs() {
	CMAKE_GEN_DIR="$1/build/myinitramfs"
	if [ ! -d $CMAKE_GEN_DIR ]; then
		mkdir -p $CMAKE_GEN_DIR
	fi
	cd $CMAKE_GEN_DIR

	sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER

	rm /mnt/boot/init
	rm /mnt/boot/sh
	cmake $1/myinitramfs $2 -DLINK_TYPE=$3 -DCMAKE_INSTALL_PREFIX=/mnt && make

	if [[ "$LINK_TYPE" == "dynamic" ]]; then
		prepare_libc_ld
		patchelf --set-interpreter $MUSL_DYN_LD init
		patchelf --set-interpreter $MUSL_DYN_LD sh
	fi

	make install

	sync

	sudo umount /mnt

	objdump -S init > $CMAKE_GEN_DIR/../initd_dasm.txt
	objdump -S sh > $CMAKE_GEN_DIR/../sh_dasm.txt

	cp init $CMAKE_GEN_DIR/../initd
	cp sh $CMAKE_GEN_DIR/../sh
}
