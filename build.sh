#! /bin/bash
THREADS=`grep -c ^processor /proc/cpuinfo`
mkdir -p ./build
cd ./build
cmake ..
make -j$THREADS
./tests_executable -s