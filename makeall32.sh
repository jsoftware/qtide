#!/bin/bash
#
# run in jqt directory

# download and install Qt dev for linux x86
# and amend the next line
QM=$HOME/Qt/5.3-linux32/5.3/gcc/bin/qmake

S=$(dirname "$0")

./clean.sh
./clean.l32

cd lib
qmake -spec linux-g++-32 && make
cd ..

cd main
qmake -spec linux-g++-32 && make
