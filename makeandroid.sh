#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

./clean.sh

cd lib
qmake -spec android-g++ && make && cp ../bin/libjqt.so ../android/libs/armeabi/.


