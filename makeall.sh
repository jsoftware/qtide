#!/bin/bash
#
# run in jqt directory

S=$(dirname "$0")

# use clang instead of g++
# export QMAKESPEC=linux-clang

QM=qmake
# QM=/usr/local/bin/qmake
hash $QM &> /dev/null
if [ $? -eq 1 ]; then
  echo 'use qmake-qt5' >&2
  QM=qmake-qt5
fi

./clean.sh
./clean.l64

cd lib
$QM && make
cd ..

cd main
$QM && make
