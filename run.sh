#!/bin/bash

clang++ ./src/main.cpp -o rt -O3 -pthread -DPRECOMPUTE_RDBL
time ./rt
convert test.ppm test.png
rm test.ppm rt
