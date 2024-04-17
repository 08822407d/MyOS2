#!/usr/bin/env bash

PROJ_ROOT=$(pwd)
DEBUG_FLAG=-DCMAKE_BUILD_TYPE=Debug

source ./scripts/make_install_kernel.sh
source ./scripts/make_install_initranfs.sh
source ./scripts/make_install_bootloader.sh

if [ ! -d "./build" ]; then
	mkdir ./build
fi

make_install_kernel $PROJ_ROOT $DEBUG_FLAG
make_install_initranfs $PROJ_ROOT $DEBUG_FLAG
make_install_bootloader $PROJ_ROOT