#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
qmake -spec win32-cross-32 && make && cp ../bin/win-x86/release/jqt1.dll ../bin/win-x86/release/jqt.dll
cd ..

cd main
qmake -spec win32-cross-32 && make


