#!/bin/sh

f() { sudo apt-get install --no-install-recommends -y "$@"; }

if [ "$1" != "raspberry32" ]; then
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
else
f libqt4-dev libqt4-svg qt4-make
f libqtwebkit-dev libqt4-opengl-dev
fi
