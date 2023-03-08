#!/bin/sh
set -e

JQTVER=$(grep "^VERSION *=" common.pri|cut -d= -f2 | tr -d " ")
echo $JQTVER

if [ "`uname`" = "Darwin" ] ; then
if [ -f bin/mac-x86_64/release/libjqt.$JQTVER.dylib ] && [ -f bin/mac-aarch64/release/libjqt.$JQTVER.dylib ] ; then
lipo \
bin/mac-x86_64/release/libjqt.$JQTVER.dylib \
bin/mac-aarch64/release/libjqt.$JQTVER.dylib \
-create -output bin/libjqt.$JQTVER.dylib
fi

if [ -f bin/mac-x86_64/release/jqt.app/Contents/MacOS/jqt ] && [ -f bin/mac-aarch64/release/jqt.app/Contents/MacOS/jqt ] ; then
lipo \
bin/mac-x86_64/release/jqt.app/Contents/MacOS/jqt \
bin/mac-aarch64/release/jqt.app/Contents/MacOS/jqt \
-create -output bin/jqt
fi

if [ -f bin/mac-x86_64/release/jqta.app/Contents/MacOS/jqta ] && [ -f bin/mac-aarch64/release/jqta.app/Contents/MacOS/jqta ] ; then
lipo \
bin/mac-x86_64/release/jqta.app/Contents/MacOS/jqta \
bin/mac-aarch64/release/jqta.app/Contents/MacOS/jqta \
-create -output bin/jqta
fi

fi

