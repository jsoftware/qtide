#!/bin/bash

S=~/dev/apps/ide/jqt
cd $S

$S/astyle.sh lib/base
$S/astyle.sh lib/high
$S/astyle.sh lib/wd

exit 0

# will call make clean
make -f lib/Makefile distclean
make -f main/Makefile distclean

exit 0

rm -rf bin/*

rm -f lib/Makefile
rm -f lib/qrc_lib.cpp
rm -rf lib/bin/*
rm -rf lib/build/*
rm -rf lib/debug/*
rm -rf lib/Info.plist
rm -rf lib/jqt.xcodeproj
rm -rf lib/release/*

rm -f main/main.o
rm -f main/Makefile
rm -rf main/bin/*
rm -rf main/debug/*
rm -rf main/Info.plist
rm -rf main/jqt.xcodeproj
rm -rf main/release/*

for i in `find $S -type d ! -path '*/\.*'`; do
 cd $i
 rm -f *.*~
done

