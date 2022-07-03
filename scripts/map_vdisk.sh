#! /usr/bin/bash

LOOPDEV=""

function mount_loop() {
	LOOPDEV=$(sudo losetup --show -f $1)
}

function map_partition() {
	sudo kpartx -av $1
}

function main() {
	mount_loop $1
	map_partition $LOOPDEV
}