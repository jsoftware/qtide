#!/bin/bash

QM=$HOME/Qt/5.4-android/5.4/android_armv7/bin/qmake
LIBS=armeabi-v7a

S=$(dirname "$0")

cd $S

./clean.sh
./clean.and

cd lib
mkdir -p ../android-libs/armeabi-v7a
ANDROID_NDK_PLATFORM=android-14 ABI=armeabi-v7a ANDROID_TARGET_ARCH=armeabi-v7a $QM -spec android-g++ && make && cp ../bin/android-armeabi-v7a/release/libjqt.so ../android-libs/armeabi-v7a/.
