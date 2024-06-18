#! /bin/bash

function prebuild_generate() {
    echo "Do prebuild things , ARCH = "$1", PWD = "$(pwd)
    KBUILD_PATH=$(pwd)"/mykernel/arch/"$1"/kbuild"
    gcc -S $KBUILD_PATH"/asm-offsets.c"
}