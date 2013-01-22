#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd main
qmake -spec android-g++ && make && cp ../bin/libqtide.so.1.0.0 ../android/libs/armeabi/libqtide.so
cd ..

cd lib
qmake -spec android-g++ && make && cp ../bin/libjqt.so.1.0.0 ../android/libs/armeabi/libjqt.so


