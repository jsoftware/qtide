#!/bin/bash
#
# run in jqt directory

S=~/dev/apps/ide/jqt

./clean.sh

cd lib
qmake -spec linux-raspi && make
cd ..

cd main
qmake -spec linux-raspi && make
