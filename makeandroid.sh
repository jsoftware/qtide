#!/bin/bash

if [ "5" = ${QT_SELECT} ]; then
QM=$HOME/Qt5.1.0/5.1.0/android_armv7/bin/qmake
LIBS=armeabi-v7a
else
QM=qmake
LIBS=armeabi
fi;

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
$QM -spec android-g++ && make && cp ../bin/libjqt.so ../android/libs/$LIBS/.


