#!/bin/bash
#
# run in jqt directory

S=~/dev/apps/ide/jqt

./clean.sh
./clean.l64

cd lib
qmake && make
cd ..

cd main
qmake && make
