#!/bin/bash
#
# run in jqt directory

S=$(dirname "$0")

./clean.sh
./clean.l32

cd lib
qmake -spec linux-cross && make
cd ..

cd main
qmake -spec linux-cross && make
