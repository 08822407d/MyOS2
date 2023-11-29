#! /usr/bin/bash
# This script get helped by document https://www.rodsbooks.com/gdisk/sgdisk-walkthrough.html

PARAM_COUNT=6
# part_no, start, size, part_type, name, fs_name

MYOS_VMDK_IDE0="/home/cheyh/vmware/myos2test/myos2test-flat.vmdk"
PART_PARAM_IDE=(
	"1, 0, +2G, ef00, \"UEFI BOOT\" , vfat"
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

script_pwd="$(dirname "$0")"
source ./$script_pwd/map_vdisk.sh

function do_part() {
	# parse and check whether partition params is right
	local allparam=("$@")
	local disk=$1
	local in_part_params=("${allparam[@]:1}")
	local gdisk_opt=()
	local mkfs_opt=()
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
		gdisk_opt[$i]="-n ${params[0]}:${params[1]}:${params[2]} -t ${params[0]}:${params[3]} -c ${params[0]}:\"${params[4]}\""
		mkfs_opt[$i]="${params[5]}"
	done

	# check target disk needs reparted
	local part_count=$[$# - 1]
	local gdisktmp=$(gdisk -l "$disk" | grep -E '^(\s+[0-9]+){3}')
	readarray -t partitions <<< "$gdisktmp"
	# if partition number not equal then delete partition table and repart
	if [ ${#partitions[@]} -ne ${#gdisk_opt[@]} ]; then
		echo "The disk need repart : $disk"
		sudo sgdisk -og "$disk"
		for line in "${gdisk_opt[@]}"; do
			# echo "sudo sgdisk $line $disk"
			eval "sudo sgdisk $line $disk"
		done
	fi

	# map vmdk partitions then format them
	map_vmdk_partitions $disk
	local devname=$(basename "$LOOPDEV""p")
	loopdevtmp=$(lsblk -f | awk -v pat="$devname" \
			'$0 ~ pat {print substr($1 " " $2, index($0, pat) + length(pat))}')
	readarray -t loopdevs <<< "$loopdevtmp"
	for line in "${loopdevs[@]}"; do
		read -ra fields <<< "$line"
		local partition_idx=${fields[0]}
		local target_type=${mkfs_opt[${fields[0]}-1]}
		# echo "$target_type"
		if [ "${fields[1]}" != "$target_type"  ]; then
			# echo "sudo mkfs -t $target_type /dev/mapper/${devname}${partition_idx}"
			eval "sudo mkfs -t $target_type /dev/mapper/${devname}${partition_idx}"
		fi
	done
	unmap_vmdk_partitions $LOOPDEV
}

# format_vdisk
do_part $MYOS_VMDK_IDE0 "${PART_PARAM_IDE[@]}"
do_part $MYOS_VMDK_NVME0 "${PART_PARAM_NVME[@]}"