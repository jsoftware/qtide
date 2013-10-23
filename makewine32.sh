#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh
./clean.w32

cd lib
wine cmd /c ../callwine32.bat
cp ../bin/win-x86/release/jqt1.dll ../bin/win-x86/release/jqt.dll
cd ..

cd main
wine cmd /c ../callwine32.bat


