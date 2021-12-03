#!/bin/bash

clang++ ./src/main.cpp -o rt -O3 -pthread
./rt
convert test.ppm test.png
rm test.ppm rt
