#!/bin/sh
set -e

# arg is Qt version, e.g. "5.15.2"
#        mac/ios/wasm

echo "pwd $(pwd)"
echo "parameters $1 $2"

if [ "$2" = "mac" ]; then
export QMAKESPEC=macx-clang
# If you use quotes, qmake considers the whole string as a single arch and that breaks some internal logic.
qmflag=QMAKE_APPLE_DEVICE_ARCHS=x86_64\ arm64
elif [ "$2" = "ios" ]; then
export QMAKESPEC=macx-ios-clang
elif [ "$2" = "wasm" ]; then
export QMAKESPEC=wasm-emscripten
fi

export PATH=$GITHUB_WORKSPACE/Qt/$1/clang_64/bin:$PATH

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ `uname` == "linux" ]; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"
QM="${QM:=qmake}"

run() {
 ./clean.l64
if [ $2 != "ios" ] && [ $2 != "wasm" ] ; then
 cd lib
 $QM "$qmflag"
 make
 cd -
 cd main
 $QM "$qmflag"
 make
 cd -
else
 cd amalgam
 $QM "$qmflag"
 make
 cd -
fi

mv bin/$2-x86_64/release $1 || mv bin/$2-aarch64/release $1 || mv bin/$2-wasm32/release $1 || true

if [ $2 != "ios" ] && [ $2 != "wasm" ] ; then
 mv $1/jqt.app/Contents/MacOS/jqt $1 || true
 mv $1/jqta.app/Contents/MacOS/jqta $1 || true
 rm -rf $1/jqt.app
 rm -rf $1/jqta.app
 cd $1
 zip --symlinks -r ../$1.zip *
 cd -
else
 cd $1
 zip --symlinks -r ../$1.zip *
 cd -
fi
ls -l "$1" || true
rm -rf "$1"
}

if [ $2 != "ios" ] && [ $2 != "wasm" ] ; then
# export NO_OPENGL=1
run jqt-"$2" "$2"
fi

export JQTSLIM=1
run jqt-"$2"-slim "$2"

#if [ -d Qt ] ; then
#if [ $2 = "wasm" ] ; then
#ios size 8.6G !!!
#find Qt/"$1" -name 'macos' -type d -delete || true
#tar -czf "$2"-Qt.tar.gz Qt
#else
#tar -czf "$2"-Qt.tar.gz Qt
#fi
#fi

./clean.l64 || true
