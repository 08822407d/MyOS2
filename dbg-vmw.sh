#!/usr/bin/env bash

ZBOOK="$HOME/vmware/grub2-test/grub2-test.vmx"
HOME_ASUS="$HOME/vmware/myos2test/myos2test.vmx"

if [ $1 == "zbook" ]; then
	vmware -x -q $ZBOOK
elif [ $1 == "asus" ]; then
	vmware -x -q $HOME_ASUS
fi