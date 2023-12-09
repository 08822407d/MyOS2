#!/usr/bin/env bash

sudo mount /dev/dm-0 /mnt -o uid=$USER,gid=$USER
sudo mount /dev/nvme1n1p1 /mnt1 -t vfat -o uid=$USER,gid=$USER
cp -r /mnt/. /mnt1
sudo umount /mnt1
sudo umount /mnt