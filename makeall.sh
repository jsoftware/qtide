#!/bin/bash
#
# run in jqt directory

S=$(dirname "$0")

QM=qmake
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
