#! /usr/bin/bash

UNSET_ENV=0
IDE_0_1="$HOME/vmware/myos2test/myos2test-flat.vmdk"
NVME_0="$HOME/vmware/myos2test/myos2test_nvme0-flat.vmdk"

source ./scripts/map_vdisk.sh

# OPTS=$(getopt -o d:: -l "" -- "$@")
# eval set -- "$OPTS"
# while true ; do
#     case "$1" in
#         -d)
# 			UNSET_ENV=1
#             shift ;;
#         --)
#             shift
#             break ;;
#         *)
#             break ;;
#     esac
# done

# echo "remainders: $@"

# if [ $UNSET_ENV -eq 1 ]; then
	map_vmdk_partitions $IDE_0_1
	map_vmdk_partitions $NVME_0
# else
# 	unmap_vmdk_partitions $IDE_0_1
# 	unmap_vmdk_partitions $NVME_0
# fi