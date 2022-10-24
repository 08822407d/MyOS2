#! /usr/bin/bash
# This script get helped by document https://www.rodsbooks.com/gdisk/sgdisk-walkthrough.html

declare -A PART_PARAM=()
PART_PARAM[2]="1:0:+1G 1:ef00 1:\"UEFI BOOT\""
PART_PARAM[1]="2:0:+2G 2:8300 2:\"ROOT\""
PART_PARAM[0]="3:0:+4G 3:ef00 3:\"FAT32 TEST\""

function part_vdisk() {
	sudo	sgdisk -og "$1"
	sudo	sgdisk -n 1:0:+1G -t 1:ef00 -c 1:"UEFI BOOT" "$1"
	sudo	sgdisk -n 2:0:+2G -t 2:8300 -c 2:"ROOT" "$1"
	ENDSECTOR=$(sgdisk -E "$1")
			sgdisk -n 3:0:"$ENDSECTOR" -t 3:ef00 -c 3:"FAT32 TEST" "$1"
			sgdisk -p "$1"
}

function format_vdisk() {
	for p in "${PART_PARAM[@]}"
	do
		arr=(${p[@]})
		echo $arr[1]
		echo "="
	done
}

format_vdisk
# part_vdisk "/home/cheyh/vmware/myos2test/myos2test-flat.vmdk"