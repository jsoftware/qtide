#!/bin/bash

QM=$HOME/Qt/5.4-android/5.4/android_armv7/bin/qmake
LIBS=${ANDROID_TARGET_ARCH}-v7a

S=$(dirname "$0")

cd $S

./clean.sh
./clean.and

cd lib
$QM -spec android-g++ && make && cp ../bin/android-arm/release/libjqt.so ../android-libs/armeabi-v7a/.
