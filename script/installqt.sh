#!/bin/sh

f() { sudo apt-get install --no-install-recommends -y "$@"; }
g() { sudo pkg_add install "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "raspberry" ] || [ "$1" = "linux" ] ; then
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "raspberry32" ] ; then
f libqt4-dev libqt4-opengl-dev libqt4-svg
elif [ "$1" = "openbsd" ] ; then
g libpulse-dev
g qtbase5-dev qtbase5-dev-tools
g qtmultimedia5-dev libqt5multimediawidgets5
g libqt5opengl5 libqt5opengl5-dev
g libqt5svg5 libqt5svg5-dev
g qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "freebsd" ] ; then
h libpulse-dev
h qtbase5-dev qtbase5-dev-tools
h qtmultimedia5-dev libqt5multimediawidgets5
h libqt5opengl5 libqt5opengl5-dev
h libqt5svg5 libqt5svg5-dev
h qtwebengine5-dev libqt5websockets5-dev
fi
