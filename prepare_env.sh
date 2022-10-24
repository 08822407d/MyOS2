#! /usr/bin/bash

ZBOOK="$HOME/vmware/grub2-test/grub2-test-flat.vmdk"
HOME_ASUS="$HOME/vmware/myos2test/myos2test-flat.vmdk"

source ./scripts/map_vdisk.sh

if [ $1 == "zbook" ]; then
	main $ZBOOK
elif [ $1 == "asus" ]; then
	main $HOME_ASUS
fi