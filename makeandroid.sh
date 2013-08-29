#!/bin/bash

if [ "5" = ${QT_SELECT} ]; then
QM=qmake
# LIBS=armeabi-v7a
LIBS=armeabi
else
QM=qmake
LIBS=armeabi
fi;

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
$QM -spec android-g++ && make && cp ../bin/libjqt.so ../android/libs/$LIBS/.


