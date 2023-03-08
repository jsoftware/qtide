#!/bin/sh
#
# run in jqt directory
# work for redhat/debian and macos
set -e

S=$HOME/dev/apps/ide/jqt
cd $S || exit 1

VER=$(grep "^JDLLVER *=" common.pri|cut -d= -f2 | tr -d " ")
JVER=$VER
# JVERNUM=$(echo $VER|tr "." "0")
JVERNUM=$VER
JQTVER=$(grep "^VERSION *=" common.pri|cut -d= -f2 | tr -d " ")
echo $JVER
echo $JVERNUM
echo $JQTVER

cop(){
# $1 PLATFORM
# $2 CPU
if [ $PLATFORM = "mac" ] ; then
SUDO=
SLIB=lib
JQTAPP=jqt.app/Contents/MacOS
JQTAAPP=jqta.app/Contents/MacOS
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
JQTAAPP=
EXT=.so
VEXT=.so.$JQTVER
PX=/usr
fi

if [ -f bin/$PLATFORM-$CPU/release/libjqt$EXT ] ; then 
$SUDO mv -f $PX/$SLIB/libjqt$VEXT /tmp/libjqt$VEXT.old.$$ || true
$SUDO cp bin/$PLATFORM-$CPU/release/libjqt$EXT $PX/$SLIB/libjqt$VEXT
$SUDO chmod 755 $PX/$SLIB/libjqt$VEXT
fi
if [ -f bin/$PLATFORM-$CPU/release/$JQTAPP/jqt ] ; then 
$SUDO mv -f $PX/bin/jqt-$VER /tmp/jqt-$VER.old.$$ || true
$SUDO cp bin/$PLATFORM-$CPU/release/$JQTAPP/jqt $PX/bin/jqt-$VER
$SUDO chmod 755 $PX/bin/jqt-$VER
fi
if [ -f bin/$PLATFORM-$CPU/release/$JQTAAPP/jqta ] ; then 
$SUDO mv -f $PX/bin/jqta-$VER /tmp/jqta-$VER.old.$$ || true
$SUDO cp bin/$PLATFORM-$CPU/release/$JQTAAPP/jqta $PX/bin/jqta-$VER
$SUDO chmod 755 $PX/bin/jqta-$VER
fi

if [ -f bin/$PLATFORM-$CPU/release/libjqt$EXT ] ; then 
mv -f ~/share/jsoftware/j$JVERNUM/bin/libjqt$EXT ~/share/jsoftware/j$JVERNUM/bin/libjqt$EXT.old || true
cp bin/$PLATFORM-$CPU/release/libjqt$EXT ~/share/jsoftware/j$JVERNUM/bin/.
fi
if [ -f bin/$PLATFORM-$CPU/release/$JQTAPP/jqt ] ; then 
mv -f ~/share/jsoftware/j$JVERNUM/bin/jqt ~/share/jsoftware/j$JVERNUM/bin/jqt.old || true
cp bin/$PLATFORM-$CPU/release/$JQTAPP/jqt ~/share/jsoftware/j$JVERNUM/bin/.
fi
if [ -f bin/$PLATFORM-$CPU/release/$JQTAAPP/jqta ] ; then 
mv -f ~/share/jsoftware/j$JVERNUM/bin/jqta ~/share/jsoftware/j$JVERNUM/bin/jqta.old || true
cp bin/$PLATFORM-$CPU/release/$JQTAPPA/jqta ~/share/jsoftware/j$JVERNUM/bin/.
fi
}

coplipo(){
# $1 PLATFORM
# $2 CPU

SUDO=
SLIB=lib
EXT=.dylib
VEXT=.$JQTVER.dylib
PX=/opt/homebrew

if [ -f bin/libjqt.$JQTVER$EXT ] ; then 
$SUDO mv -f $PX/$SLIB/libjqt$VEXT /tmp/libjqt$VEXT.old.$$ || true
$SUDO cp bin/libjqt.$JQTVER$EXT $PX/$SLIB/libjqt$VEXT
$SUDO chmod 755 $PX/$SLIB/libjqt$VEXT
fi
if [ -f bin/jqt ] ; then 
$SUDO mv -f $PX/bin/jqt-$VER /tmp/jqt-$VER.old.$$ || true
$SUDO cp bin/jqt $PX/bin/jqt-$VER
$SUDO chmod 755 $PX/bin/jqt-$VER
fi
if [ -f bin/jqta ] ; then 
$SUDO mv -f $PX/bin/jqta-$VER /tmp/jqta-$VER.old.$$ || true
$SUDO cp bin/jqta $PX/bin/jqta-$VER
$SUDO chmod 755 $PX/bin/jqta-$VER
fi

if [ -f bin/libjqt.$JQTVER$EXT ] ; then 
mv -f ~/share/jsoftware/j$JVERNUM/bin/libjqt$EXT ~/share/jsoftware/j$JVERNUM/bin/libjqt$EXT.old || true
cp bin/libjqt.$JQTVER$EXT ~/share/jsoftware/j$JVERNUM/bin/libjqt$EXT
fi
if [ -f bin/jqt ] ; then 
mv -f ~/share/jsoftware/j$JVERNUM/bin/jqt ~/share/jsoftware/j$JVERNUM/bin/jqt.old || true
cp bin/jqt ~/share/jsoftware/j$JVERNUM/bin/.
fi
if [ -f bin/jqta ] ; then 
mv -f ~/share/jsoftware/j$JVERNUM/bin/jqta ~/share/jsoftware/j$JVERNUM/bin/jqta.old || true
cp bin/jqta ~/share/jsoftware/j$JVERNUM/bin/.
fi
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

if { [ -f bin/mac-x86_64/release/libjqt.$JQTVER.dylib ] && [ -f bin/mac-aarch64/release/libjqt.$JQTVER.dylib ] ; } || \
   { [ -d bin/mac-x86_64/release/jqt.app ] && [ -d bin/mac-aarch64/release/jqt.app ] ; } || \
   { [ -d bin/mac-x86_64/release/jqta.app ] && [ -d bin/mac-aarch64/release/jqta.app ] ; } ; then 
./maclipo.sh
coplipo

else

if [ -f bin/mac-x86_64/release/libjqt.$JQTVER.dylib ]  ; then 
PLATFORM=mac
CPU=x86_64
cop
fi

if [ -f bin/mac-aarch64/release/libjqt.$JQTVER.dylib ] ; then 
PLATFORM=mac
CPU=arm64
cop
fi

fi

fi
