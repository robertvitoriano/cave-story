#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
fi

if [ -f "Cavestory.out" ]; then
    rm Cavestory.out
fi

cd build

cmake -G Ninja ..

ninja

cd ..

rm -rf build

./Cavestory.out
