#!/bin/bash

clang++ ./src/*.cpp -o rt -O3 -pthread -DPRECOMPUTE_RDBL -DUSE_SSE -DINTERACTIVE -lsfml-graphics -lsfml-window -lsfml-system
./rt
convert test.ppm test.png
rm test.ppm rt
