#!/usr/bin/env bash

# VMW_BOOT="$HOME/vmware/myos2test/myos2test.vmx"
VMW_BOOT="$HOME/vmware/myos2test/myos2test_dbg.vmx"

# if [ $1 == "zbook" ]; then
# 	vmware -x -q $ZBOOK
# elif [ $1 == "asus" ]; then
# 	vmware -x -q $VMW_BOOT
# fi

vmware -x -q $VMW_BOOT