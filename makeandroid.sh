#!/bin/bash

# QM=qmake
QM=$HOME/Qt/5.1.1/5.1.1/android-armv5/bin/qmake
LIBS=$ANDROID_TARGET_ARCH

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
$QM -spec android-g++ && make && cp ../bin/libjqt.so ../android/libs/$LIBS/.


