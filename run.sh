#!/bin/bash

clang++ main.cpp -o rt -O3
./rt
convert test.ppm test.png
rm test.ppm rt
