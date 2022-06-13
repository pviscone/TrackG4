#! /bin/bash
THREADS=`grep -c ^processor /proc/cpuinfo`
mkdir -p ./build
cd ./build
rm ./output*.root
cmake ..
make -j$THREADS
./CosmicMuSimulation
hadd ./output.root ./output*.root
