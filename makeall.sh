#!/bin/bash
#
# run in jqt directory

S=$(dirname "$0")

./clean.sh
./clean.l64

cd lib
qmake && make
cd ..

cd main
qmake && make
