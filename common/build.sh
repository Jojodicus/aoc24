#! /bin/sh

set -e

mkdir -p .build
cd .build
cmake ..
make
cp aoc ..
