#!/bin/bash

S=~/dev/apps/ide/jqt
cd $S

rm -rf $S/bin/*
rm -rf $S/main/bin/*
rm -rf $S/lib/bin/*
rm -rf $S/lib/build/*
rm -f lib/Makefile
rm -f lib/qrc_lib.cpp

rm -f main/Makefile
rm -f main/main.o

$S/astyle.sh lib/base
$S/astyle.sh lib/high
$S/astyle.sh lib/high
$S/astyle.sh lib/wd

for i in `find $S -type d ! -path '*/\.*'`; do
 cd $i
 rm -f *.*~
done

