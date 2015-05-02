#!/bin/bash

QM=$HOME/Qt/5.4-android/5.4/android_x86/bin/qmake
LIBS=x86

S=$(dirname "$0")

cd $S

./clean.sh
./clean.andx

cd lib
mkdir -p ../android-libs/x86
ANDROID_NDK_PLATFORM=android-14 ABI=x86 ANDROID_TARGET_ARCH=x86 $QM -spec android-g++ && make && cp ../bin/android-x86/release/libjqt.so ../android-libs/x86/.
