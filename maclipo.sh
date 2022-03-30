#!/bin/sh

JQTVER=2.0.3

if [ "`uname`" = "Darwin" ] ; then
if [ -f bin/mac-x86_64/release/libjqt.$JQTVER.dylib ] && [ -f bin/mac-arm64/release/libjqt.$JQTVER.dylib ] ; then
lipo \
bin/mac-x86_64/release/libjqt.$JQTVER.dylib \
bin/mac-arm64/release/libjqt.$JQTVER.dylib \
-create -output bin/libjqt.$JQTVER.dylib
fi

if [ -f bin/mac-x86_64/release/jqt.app/Contents/MacOS/jqt ] && [ -f bin/mac-arm64/release/jqt.app/Contents/MacOS/jqt ] ; then
lipo \
bin/mac-x86_64/release/jqt.app/Contents/MacOS/jqt \
bin/mac-arm64/release/jqt.app/Contents/MacOS/jqt \
-create -output bin/jqt
fi

fi

