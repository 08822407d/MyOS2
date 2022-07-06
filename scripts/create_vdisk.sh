#! /usr/bin/bash
# This script get helped by document https://www.rodsbooks.com/gdisk/sgdisk-walkthrough.html

declare -A PART_PARAM=(	[0]="1:0:+1G 1:ef00 1:\"UEFI BOOT\""
						[1]="2:0:+2G 2:8300 2:\"ROOT\""
						[2]="3:0:+4G 3:ef00 3:\"FAT32 TEST\"")

function part_vdisk() {
	sudo	sgdisk -og "$1"
			sgdisk -n 1:0:+1G -t 1:ef00 -c 1:"UEFI BOOT" "$1"
			sgdisk -n 2:0:+2G -t 2:8300 -c 3:"ROOT" "$1"
	ENDSECTOR=$(sgdisk -E "$1")
			sgdisk -n 3:0:"$ENDSECTOR" -t 3:ef00 -c 2:"FAT32 TEST" "$1"
			sgdisk -p "$1"
}

function format_vdisk() {
	for p in "${PART_PARAM[@]}"
	do
		echo ${p[0]}
	done
}

format_vdisk