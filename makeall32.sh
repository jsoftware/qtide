#!/bin/bash
#
# run in jqt directory

S=~/dev/apps/ide/jqt

./clean.sh
./clean.l32

cd lib
qmake -spec linux-cross && make
cd ..

cd main
qmake -spec linux-cross && make
