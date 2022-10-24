#!/usr/bin/env bash

if [ ! -d "./build" ]; then
	mkdir ./build
fi

rm -rf ./build/*
cd build && cmake .. && make && bash ../install.sh