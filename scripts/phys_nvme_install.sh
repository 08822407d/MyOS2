#!/usr/bin/env bash


function install_to_physdisk() {
    sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
    sudo mount $1 /mnt1 -t vfat -o uid=$USER,gid=$USER
    rm -rf /mnt1/*
    cp -r /mnt/. /mnt1
    sudo umount /mnt1
    sudo umount /mnt
}