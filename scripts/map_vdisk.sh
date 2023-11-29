#! /usr/bin/bash

LOOPDEV=""

function mount_loop() {
	LOOPDEV=$(sudo losetup --show -f $1)
}

function umount_loop() {
	sudo losetup -d $1
}

function map_partition() {
	sudo kpartx -av $1 > /dev/null 2>&1
}

function unmap_partition() {
	sudo kpartx -d $1
}

function map_vmdk_partitions() {
	mount_loop $1
	map_partition $LOOPDEV
}

function unmap_vmdk_partitions() {
	unmap_partition $1
	umount_loop $1
}