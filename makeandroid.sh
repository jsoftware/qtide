#!/bin/bash

# QM=qmake
QM=$HOME/Qt/5.2.0/5.2.0/android_armv5/bin/qmake
LIBS=$ANDROID_TARGET_ARCH

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
$QM -spec android-g++ && make
