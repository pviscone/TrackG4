#! /bin/bash
THREADS=`grep -c ^processor /proc/cpuinfo`
mkdir -p ./build
cd ./build
rm ./output*.root
cmake ..
make -j$THREADS
#./tests_executable -s
./CosmicMuSimulation run.mac
hadd ./output.root ./output*.root
