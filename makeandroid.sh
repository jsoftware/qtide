#!/bin/bash

# QM=qmake
QM=$HOME/Qt/5.3.0/5.3/android_armv5/bin/qmake
LIBS=$ANDROID_TARGET_ARCH

S=$(dirname "$0")

cd $S

./clean.sh
./clean.and

cd lib
$QM -spec android-g++ && make
