#!/bin/sh
#
# run in jqt directory

S=$(dirname "$0")

# use clang instead of g++
# export QMAKESPEC=linux-clang

if [ "`uname`" = "Darwin" ] && [ "$QMAKESPEC"x = "x" ] ; then
 export QMAKESPEC=macx-g++
fi

# QM=/usr/local/bin/qmake
QM="${QM:=qmake}"
hash $QM &> /dev/null
if [ $? -eq 1 ]; then
  echo 'use qmake-qt5' >&2
  QM=qmake-qt5
fi

# old version of astyle in raspbian
./clean.sh || true
./clean.l64

cd lib
$QM && make
cd ..

cd main
$QM && make
