#!/usr/bin/env bash

PROJ_ROOT=$(pwd)

source ./scripts/make_install_kernel.sh
source ./scripts/make_install_initranfs.sh
source ./scripts/make_install_bootloader.sh

if [ ! -d "./build" ]; then
	mkdir ./build
fi

make_install_kernel $PROJ_ROOT
make_install_initranfs $PROJ_ROOT
make_install_bootloader $PROJ_ROOT