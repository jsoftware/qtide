#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
qmake -spec win32-cross-32 && make && cp bin/jqt.dll ../bin/jqt.dll
cd ..

cd main
qmake -spec win32-cross-32 && make && cp bin/jqt.exe ../bin/.


