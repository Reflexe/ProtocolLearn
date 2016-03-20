#!/bin/sh

THREADS_NUMBER=$(grep 'processor' /proc/cpuinfo | wc -l)
export CXX=/usr/bin/clang++
export CC=/usr/bin/clang

echo "Starting... threads number: $THREADS_NUMBER"
cmake -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ ../ && make -j $THREADS_NUMBER
