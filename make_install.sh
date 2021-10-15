#!/usr/bin/env bash

rm -rf ./build/*
cd build && cmake .. && make && bash ../install.sh