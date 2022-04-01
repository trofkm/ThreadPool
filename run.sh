#! /bin/bash


[ -d build ] || mkdir build

cd build
printf "\nBuilding project...\n"
cmake ..

make
printf "Launching executable...\n\n"
./threadPool