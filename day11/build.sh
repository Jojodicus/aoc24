#! /bin/sh

set -e

mkdir -p .build
cd .build
cmake -DOMP=1 ..
make
cp aoc ..
