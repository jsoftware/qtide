#!/bin/bash

QM=qmake
LIBS=$(ANDROID_TARGET_ARCH)

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
$QM -spec android-g++ && make && cp ../bin/libjqt.so ../android/libs/$LIBS/.


