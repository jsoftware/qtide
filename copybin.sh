#!/bin/sh
#
# run in jqt directory
# work for redhat/debian and macos
set -e

S=$HOME/dev/apps/ide/jqt
cd $S || exit 1

VER=9.04
JVER=904
JQTVER=2.0.3

cop(){
# $1 PLATFORM
# $2 CPU
if [ $PLATFORM = "mac" ] ; then
SUDO=
SLIB=lib
JQTAPP=jqt.app/Contents/MacOS
EXT=.dylib
VEXT=.$JQTVER.dylib
if [ $CPU = "arm64" ] ; then
PX=/opt/homebrew
else
PX=/usr/local
fi
else
SUDO=sudo
if [ $ID = "redhat" ] ; then
SLIB=lib64
else
SLIB=lib/x86_64-linux-gnu
fi
JQTAPP=
EXT=.so
VEXT=.so.$JQTVER
PX=/usr
fi

$SUDO mv -f $PX/$SLIB/libjqt$VEXT /tmp/libjqt$VEXT.old || true
$SUDO mv -f $PX/bin/jqt-$VER /tmp/jqt-$VER.old || true
$SUDO cp bin/$PLATFORM-$CPU/release/libjqt$EXT $PX/$SLIB/libjqt$VEXT
$SUDO cp bin/$PLATFORM-$CPU/release/$JQTAPP/jqt $PX/bin/jqt-$VER
$SUDO chmod 755 $PX/$SLIB/libjqt$VEXT
$SUDO chmod 755 $PX/bin/jqt-$VER

mv -f ~/share/jsoftware/j$JVER/bin/libjqt$EXT ~/share/jsoftware/j$JVER/bin/libjqt$EXT.old || true
mv -f ~/share/jsoftware/j$JVER/bin/jqt ~/share/jsoftware/j$JVER/bin/jqt.old || true
cp bin/$PLATFORM-$CPU/release/libjqt$EXT ~/share/jsoftware/j$JVER/bin/.
cp bin/$PLATFORM-$CPU/release/$JQTAPP/jqt ~/share/jsoftware/j$JVER/bin/.
}

coplipo(){
# $1 PLATFORM
# $2 CPU

SUDO=
SLIB=lib
EXT=.dylib
VEXT=.$JQTVER.dylib
PX=/opt/homebrew

$SUDO mv -f $PX/$SLIB/libjqt$VEXT /tmp/libjqt$VEXT.old || true
$SUDO mv -f $PX/bin/jqt-$VER /tmp/jqt-$VER.old || true
echo cp bin/libjqt.$JQTVER$EXT $PX/$SLIB/libjqt$VEXT
echo cp bin/jqt $PX/bin/jqt-$VER
$SUDO cp bin/libjqt.$JQTVER$EXT $PX/$SLIB/libjqt$VEXT
$SUDO cp bin/jqt $PX/bin/jqt-$VER
$SUDO chmod 755 $PX/$SLIB/libjqt$VEXT
$SUDO chmod 755 $PX/bin/jqt-$VER

mv -f ~/share/jsoftware/j$JVER/bin/libjqt$EXT ~/share/jsoftware/j$JVER/bin/libjqt$EXT.old || true
mv -f ~/share/jsoftware/j$JVER/bin/jqt ~/share/jsoftware/j$JVER/bin/jqt.old || true
cp bin/libjqt.$JQTVER$EXT ~/share/jsoftware/j$JVER/bin/libjqt$EXT
cp bin/jqt ~/share/jsoftware/j$JVER/bin/.
}

if [ "`uname`" = "Linux" ] ; then
if [ -f /etc/redhat-release ]; then
ID='redhat'
else
ID='debian'
fi
PLATFORM=linux
CPU=x86_64
cop

elif [ "`uname`" = "Darwin" ] ; then

if [ -f bin/mac-x86_64/release/libjqt.$JQTVER.dylib ] && [ -f bin/mac-arm64/release/libjqt.$JQTVER.dylib ] ; then 
./maclipo.sh
coplipo

else

if [ -f bin/mac-x86_64/release/libjqt.$JQTVER.dylib ]  ; then 
PLATFORM=mac
CPU=x86_64
cop
fi

if [ -f bin/mac-arm64/release/libjqt.$JQTVER.dylib ] ; then 
PLATFORM=mac
CPU=arm64
cop
fi

fi

fi
