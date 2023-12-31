#!/bin/sh
set -e

A=$HOME/dev/jsource/android
B=$HOME/dev/apps/ide/jqt/android
C=$HOME/jal/base9/release
D=$HOME/jal/addons

f1(){
rm -rf $B/assets
mkdir -p $B/assets
cp -r $C/install $B/assets/jlibrary
mkdir -p $B/assets/jlibrary/bin
cp $C/../config/profile*.ijs $B/assets/jlibrary/bin
mkdir -p $B/assets/jlibrary/addons
cp -r $D/dev $B/assets/jlibrary/addons/.
mkdir -p $B/assets/jlibrary/addons/ide/qt
cp $D/ide/qt/*.ijs $B/assets/jlibrary/addons/ide/qt/.
cp -r $D/ide/qt/config $B/assets/jlibrary/addons/ide/qt/.
cp -r $D/ide/qt/images $B/assets/jlibrary/addons/ide/qt/.
find $B -name '.git' -delete
find $B -name 'manifest.ijs' -delete
}

f1 || true

f2(){
mkdir -p $B/libs/$1
cp $A/libs-bare/$1/*.so $B/libs/$1/.
cp $A/libs/$1/libtsdll.so $B/libs/$1/.
mkdir -p $B/lib/$1
cp $A/obj/local/$1/*.a $B/lib/$1/.
}

f2 "arm64-v8a" || true
f2 "armeabi-v7a" || true
f2 "x86_64" || true
f2 "x86" || true
