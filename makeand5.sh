#!/bin/bash

QM=$HOME/Qt/5.4-android/5.4/android_armv5/bin/qmake
LIBS=$ANDROID_TARGET_ARCH

S=$(dirname "$0")

cd $S

./clean.sh
./clean.and5

cd lib
mkdir -p ../android-libs/armeabi
ABI=armeabi $QM -spec android-g++ && make && cp ../bin/android-armeabi/release/libjqt.so ../android-libs/armeabi/.
