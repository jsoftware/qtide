#!/bin/sh

f() { sudo apt-get install -y "$@"; }

if [ "$1" != "raspberry32" ]; then
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
else
f libpulse-dev
f qtbase4-dev qtbase4-dev-tools
f qtmultimedia4-dev libqt4multimediawidgets4
f libqt4opengl4 libqt4opengl4-dev
f libqt4svg4 libqt4svg4-dev
f qtwebkit4-dev libqt4webkit4-dev
fi
