#! /usr/bin/bash
# This script get helped by document https://www.rodsbooks.com/gdisk/sgdisk-walkthrough.html

function part_vdisk() {
	sudo	sgdisk -og "$1"
			sgdisk -n 1:0:+1G -t 1:ef00 -c 1:"UEFI BOOT" "$1"
			sgdisk -n 2:0:+2G -t 2:ef00 -c 3:"ROOT" "$1"
	ENDSECTOR=$(sgdisk -E "$1")
			sgdisk -n 3:0:"$ENDSECTOR" -t 3:8300 -c 2:"FAT32 TEST" "$1"
			sgdisk -p "$1"
}

function format_vdisk() {

}