#!/bin/sh
#
# arg is Qt version, e.g. "5.15.2"

export QMAKESPEC=macx-clang
export PATH=$GITHUB_WORKSPACE/qt/Qt/$1/clang_64/bin:$PATH

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
 zip --symlinks -r $1.zip $1
}

run jqt-mac

export JQTSLIM=1
run jqt-mac-slim
