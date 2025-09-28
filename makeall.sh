#!/bin/sh
#
# run in jqt directory
set -e

cd "$(dirname "$0")"

VER=6.9.2

if [ "$1" != "noclean" ] ; then
./clean.sh || true
./clean.l64
fi

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ `uname` = "Linux" ] ; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"

# use clang instead of g++
# export QMAKESPEC=linux-clang

if [ "`uname`" = "Darwin" ] && [ "$QMAKESPEC"x = "x" ] ; then
 export QMAKESPEC=macx-clang
fi

if [ $QMAKESPEC = "macx-clang" ] ; then
 qmflag=QMAKE_APPLE_DEVICE_ARCHS=x86_64\ arm64
fi

if [ "`uname`" = "FreeBSD" ] && [ "$QMAKESPEC"x = "x" ] ; then
# export QMAKESPEC=freebsd-clang
 QM=/usr/local/bin/qmake-qt5
fi

if [ "`uname`" = "OpenBSD" ] && [ "$QMAKESPEC"x = "x" ] ; then
 QM=/usr/local/bin/qmake-qt5
fi

# QM=/usr/local/bin/qmake
QM="${QM:=qmake}"
hash $QM &> /dev/null
if [ $? -eq 1 ]; then
  echo 'use qmake-qt5' >&2
  QM=qmake-qt5
fi

if [ $QMAKESPEC != "macx-ios-clang" ] && [ $QMAKESPEC != "wasm-emscripten" ] && [ $QMAKESPEC != "android-clang" ] ; then

cd lib
$QM "$qmflag" && make
cd -

cd main
$QM "$qmflag" && make
cd -

else

if [ "$QMAKESPEC" = "macx-ios-clang" ] ; then
cd amalgam && \
$QM "$qmflag" && make && \
cd ..

elif [ "$QMAKESPEC" = "wasm-emscripten" ] ; then
cd amalgam && \
$QM "$qmflag" && make && \
cd .. && \
cp main/jgreen.ico bin/wasm-wasm32/release/favicon.ico

elif [ "$QMAKESPEC" = "android-clang" ] ; then

rm -rf android-build/assets && \
mkdir -p android-build/assets && \
cp -r android/assets/jlibrary android-build/assets/. && \
mkdir -p android-build/libs/$ABI && \
cp android/libs/$ABI/*.so android-build/libs/$ABI/. && \
cd amalgam && \
$QM "$qmflag" && make && \
cp ../bin/android-$ABI/release/libjqta_$ABI.so ../android-build/libs/$ABI && \
make apk_install_target ../android-build && \
unset JAVA_HOME && \
$HOME/Qt/$VER/macos/bin/androiddeployqt \
 --output ../android-build \
 --input ./android-jqta-deployment-settings.json \
 --android-platform android-35 \
 --release \
 --sign $HOME/.android/release-key.keystore jandroid --storepass $JANDROIDSTOREPASS --keypass $JANDROIDKEYPASS && \
cd .. && \
mkdir -p bin/android-$ABI/release && \
cp android-build/build/outputs/apk/release/android-build-release-signed.apk bin/android-$ABI/release/jqta.apk

fi

fi
