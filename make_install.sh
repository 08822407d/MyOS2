#!/usr/bin/env bash

PROJ_ROOT=$(pwd)
DEBUG_FLAG=-DCMAKE_BUILD_TYPE=Debug
TARGET_ARCH=x86_64

# source ./scripts/prebuild_generate.sh
source ./scripts/make_install_kernel.sh
source ./scripts/make_install_initranfs.sh
source ./scripts/make_install_bootloader.sh

if [ ! -d "./build" ]; then
	mkdir ./build
fi
rm -rf ./build/*


# prebuild_generate $TARGET_ARCH

make_install_kernel $PROJ_ROOT $DEBUG_FLAG $TARGET_ARCH
make_install_initranfs $PROJ_ROOT $DEBUG_FLAG $TARGET_ARCH
make_install_bootloader $PROJ_ROOT $TARGET_ARCH

# bash ./configure --prefix=/home/cheyh/projs/musl/build --enable-debug --enable-static --disable-shared --exec-prefix=/home/cheyh/projs/musl/build/exec --syslibdir=/home/cheyh/projs/musl/build/sys --disable-optimize