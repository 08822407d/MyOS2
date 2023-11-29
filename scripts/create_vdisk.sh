#! /usr/bin/bash
# This script get helped by document https://www.rodsbooks.com/gdisk/sgdisk-walkthrough.html

PARAM_COUNT=6
# part_no, start, size, part_type, name, fs_name

MYOS_VMDK_IDE0="/home/cheyh/vmware/myos2test/myos2test-flat.vmdk"
PART_PARAM_IDE=(
	"1, 0, +1G, ef00, \"UEFI BOOT\" , vfat"
	"2, 0, +2G, 8300, \"ROOT\"      , ext2"
	"3, 0, 0  , ef00, \"FAT32 TEST\", vfat"
)

MYOS_VMDK_NVME0="/home/cheyh/vmware/myos2test/myos2test_nvme0-flat.vmdk"
PART_PARAM_NVME=(
	"1, 0, +2G, ef00, \"UEFI BOOT\" , vfat"
	"2, 0, +8G, 8300, \"ROOT\"      , ext2"
	"3, 0, +8G, 8300, \"HOME\"      , ext4"
	"4, 0, 0  , ef00, \"FAT32 TEST\", vfat"
)

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

function do_part() {
	# parse and check whether partition params is right
	local allparam=("$@")
	local disk=$1
	local in_part_params=("${allparam[@]:1}")
	local gdisk_opt=()
	for line in "${in_part_params[@]}"; do
		IFS=',' read -a params <<< "$line"
		if [ ${#params[@]} -ne $PARAM_COUNT ]; then
			echo "Wrong param count : $line"
			exit 1
		fi
	done
	# handle partition param array to trim blank char
	for i in "${!in_part_params[@]}"; do
		local params=()
		IFS=',' read -ra tmparr <<< "${in_part_params[$i]}"
		for j in "${!tmparr[@]}"; do
			params[$j]=$(echo "${tmparr[$j]}" | xargs)
		done
		gdisk_opt[$i]=" -n ${params[0]}:${params[1]}:${params[2]} -t ${params[0]}:${params[3]} -c ${params[0]}:\"${params[4]}\""
	done

	# check target disk needs reparted
	local part_count=$[$# - 1]
	local gdisktmp=$(gdisk -l "$disk" | grep -E '^(\s+[0-9]+){3}')
	readarray -t partitions <<< "$gdisktmp"
	# if partition number not equal then delete partition table and repart
	if [ ${#partitions[@]} -ne ${#part_params[@]} ]; then
		echo "The disk need repart : $disk"
		sudo sgdisk -og "$disk"
		for line in "${gdisk_opt[@]}"; do
			# echo "$line"
			sudo sgdisk "$line $disk"
		done
	fi
}

# format_vdisk
# part_vdisk "/home/cheyh/vmware/myos2test/myos2test-flat.vmdk"
do_part $MYOS_VMDK_NVME0 "${PART_PARAM_NVME[@]}"