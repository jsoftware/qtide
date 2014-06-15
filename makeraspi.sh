#!/bin/bash
#
# run in jqt directory

S=$(dirname "$0")

./clean.sh
./clean.rpi

cd lib
qmake -spec linux-raspi && make
cd ..

cd main
qmake -spec linux-raspi && make
