#!/bin/sh
set -e

# arg is  mac/ios/wasm Qt version, e.g. "5.15.2"
#       

echo "pwd $(pwd)"
echo "parameters $1 $2"

export QTLIBVER="$2"

if [ "$1" = "mac" ]; then
export QMAKESPEC=macx-clang
# If you use quotes, qmake considers the whole string as a single arch and that breaks some internal logic.
qmflag=QMAKE_APPLE_DEVICE_ARCHS=x86_64\ arm64
elif [ "$1" = "ios" ]; then
export QMAKESPEC=macx-ios-clang
elif [ "$1" = "wasm" ]; then
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
P=jqt-"$1"
if [ "$JQTSLIM" = "1" ] ; then
P="$P"-slim
fi

 ./clean.l64
if [ $1 != "ios" ] && [ $1 != "wasm" ] ; then
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

mv bin/"$1"-x86_64/release $P || mv bin/"$1"-aarch64/release $P || mv "$1"-wasm32/release $P || true

if [ $1 != "ios" ] && [ $1 != "wasm" ] ; then
 mv $P/jqt.app/Contents/MacOS/jqt $P || true
 mv $P/jqta.app/Contents/MacOS/jqta $P || true
 rm -rf $P/jqt.app
 rm -rf $P/jqta.app
 cd $P
 zip --symlinks -r ../$P.zip *
 cd -
else
 cd $P
 zip --symlinks -r ../$P.zip *
 cd -
fi
ls -l "$P" || true
rm -rf "$P"
}

if [ $1 != "ios" ] && [ $1 != "wasm" ] ; then
# export NO_OPENGL=1
run "$1"
fi

export JQTSLIM=1
run "$1"

#if [ -d Qt ] ; then
#if [ $1 = "wasm" ] ; then
#ios size 8.6G !!!
#find Qt/"$1" -name 'macos' -type d -delete || true
#tar -czf "$1"-Qt.tar.gz Qt
#else
#tar -czf "$1"-Qt.tar.gz Qt
#fi
#fi

./clean.l64 || true
