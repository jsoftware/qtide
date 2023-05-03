#!/bin/sh
set -e

# arg is Qt version, e.g. "5.15.2"
#        mac/ios/masm

if [ "$2" = "mac" ]; then
export QMAKESPEC=macx-clang
qmflag=QMAKE_APPLE_DEVICE_ARCHS=x86_64\ arm64
elif [ "$2" = "ios" ]; then
export QMAKESPEC=macx-ios-clang
elif [ "$2" = "wasm" ]; then
export QMAKESPEC=wasm-emscripten
fi

export PATH=$GITHUB_WORKSPACE/qt/Qt/$1/clang_64/bin:$PATH

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ `uname` == "linux" ]; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"
QM="${QM:=qmake}"

run() {
 ./clean.l64
if [ "$QMAKESPEC" != "macx-ios-clang" ] && [ "$QMAKESPEC" != "wasm-emscripten" ] ; then

# If you use quotes, qmake considers the whole string as a single arch and that breaks some internal logic.
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

if [ $2 != "ios" ] && [ $2 != "wasm" ] ; then
 mv bin/$2-x86_64/release $1 || mv bin/$2-aarch64/release $1 || true
else
 mv bin/$2-x86_64/release $1 || mv bin/$2-aarch64/release $1 || true
fi
 mv $1/jqt.app/Contents/MacOS/jqt $1 || true
 mv $1/jqta.app/Contents/MacOS/jqta $1 || true
 rm -rf $1/jqt.app
 rm -rf $1/jqta.app
 cd $1
 zip --symlinks -r ../$1.zip *
 cd -
}

if [ -d qt ] ; then
if [ "$QMAKESPEC" = "macx-ios-clang" ] || [ "$QMAKESPEC" = "wasm-emscripten" ] ; then
find qt -name 'macos' -type d -delete || true
cd qt && tar -czf ../"$2"-Qt.tar.gz Qt
else
cd qt && tar -czf ../"$2"-Qt.tar.gz Qt
cd -
fi
fi

if [ "$QMAKESPEC" = "macx-ios-clang" ] || [ "$QMAKESPEC" = "wasm-emscripten" ] ; then
export JQTSLIM=1
run jqt-"$2"-slim "$2"
else
run jqt-"$2" "$2"

export JQTSLIM=1
run jqt-"$2"-slim "$2"
fi

./clean.l64 || true
