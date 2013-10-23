#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh
./clean.w32

cd lib
wine64 cmd /c ../callwine64.bat
cp ../bin/win-x86_64/release/jqt1.dll ../bin/win-x86_64/release/jqt.dll
cd ..

cd main
wine64 cmd /c ../callwine64.bat


