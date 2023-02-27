#!/bin/sh
set -e

# arg is Qt version, e.g. "5.15.2"

export QMAKESPEC=macx-clang
export PATH=$GITHUB_WORKSPACE/qt/Qt/$1/clang_64/bin:$PATH

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ `uname` == "linux" ]; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"

run() {
 ./clean.l64
 cd lib
 qmake QMAKE_APPLE_DEVICE_ARCHS="x86_64 arm64"
 make
 cd ../main
 qmake QMAKE_APPLE_DEVICE_ARCHS="x86_64 arm64"
 make
 cd ..
 mv bin/mac-x86_64/release $1
 mv $1/jqt.app/Contents/MacOS/jqt $1
 rm -rf $1/jqt.app
 cd $1
 zip --symlinks -r ../$1.zip *
 cd ..
}

run jqt-mac

export JQTSLIM=1
run jqt-mac-slim

./clean.l64 || true
