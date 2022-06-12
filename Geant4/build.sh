#! /bin/bash
THREADS=`grep -c ^processor /proc/cpuinfo`
mkdir -p ./build
cd ./build
cmake ..
make -j$THREADS
rm ./output*.root
./CosmicMuSimulation
hadd ./output.root ./output*.root
