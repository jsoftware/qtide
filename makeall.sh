#!/bin/sh
#
# run in jqt directory
set -e

S=$(dirname "$0")

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

if [ $QMAKESPEC != "macx-ios-clang" ] && [ $QMAKESPEC != "wasm-emscripten" ] ; then

cd lib
$QM "$qmflag" && make
cd -

cd main
$QM "$qmflag" && make
cd -

else

cd amalgam
$QM "$qmflag" && make
cd -

fi
