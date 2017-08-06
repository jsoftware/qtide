#!/bin/bash
#
# run in jqt directory

S=$(dirname "$0")

# old version of astyle in raspbian
# ./clean.sh
./clean.rpi3

cd lib
# qmake -spec linux-raspi && make
qmake && make
cd ..

cd main
# qmake -spec linux-raspi && make
qmake && make
