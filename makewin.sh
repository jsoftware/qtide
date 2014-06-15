#!/bin/bash

S=$(dirname "$0")

cd $S

./clean.sh
./clean.w32

cd lib
qmake -spec win32-cross-32 && make && cp ../bin/win-x86/release/jqt1.dll ../bin/win-x86/release/jqt.dll
cd ..

cd main
qmake -spec win32-cross-32 && make


