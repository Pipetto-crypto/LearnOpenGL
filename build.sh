#!/bin/bash

src=src/$1
out=$(basename -- $src | cut -f1 -d".")

mkdir -p build

clang++ -I"include" -o build/$out $src src/util/shader-parser.cpp -lGL -lGLEW -lglfw  

