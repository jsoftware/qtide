#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
qmake -spec win32-cross && make && cp bin/jqt1.dll ../bin/jqt.dll
cd ..

cd main
qmake -spec win32-cross && make && cp bin/jqt.exe ../bin/.


