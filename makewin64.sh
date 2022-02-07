#!/bin/sh

S=$(dirname "$0")

cd $S

./clean.sh
./clean.w64

cd lib
qmake -spec win32-cross && make && cp ../bin/win-x86_64/release/jqt1.dll ../bin/win-x86_64/release/jqt.dll
cd ..

cd main
qmake -spec win32-cross && make


