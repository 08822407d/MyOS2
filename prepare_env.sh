#! /usr/bin/bash

# ZBOOK="$HOME/vmware/grub2-test/grub2-test-flat.vmdk"
IDE_0_1="$HOME/vmware/myos2test/myos2test-flat.vmdk"
NVME_0="$HOME/vmware/myos2test/myos2test_nvme0-flat.vmdk"

source ./scripts/map_vdisk.sh

# if [ $1 == "zbook" ]; then
# 	main $ZBOOK
# elif [ $1 == "asus" ]; then
# 	main $HOME_ASUS
# fi

main $IDE_0_1
main $NVME_0