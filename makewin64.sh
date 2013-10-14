#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
qmake -spec win32-cross && make && cp ../bin/win-x86_64/release/jqt1.dll ../bin/win-x86_64/release/jqt.dll
cd ..

cd main
qmake -spec win32-cross && make


