#!/bin/sh
set -e

f() { sudo apt-get install --no-install-recommends -y "$@"; }
g() { sudo pkg_add "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux" ] ; then
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "raspberry" ] ; then
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "raspberry32" ] ; then
f libqt4-dev libqt4-opengl-dev libqt4-svg
elif [ "$1" = "openbsd" ] ; then
g qtbase qtmultimedia qtsvg qttools qtwebengine qtwebsockets
elif [ "$1" = "freebsd" ] ; then
h qt5-qmake qt5-buildtools qt5-core
h qt5-gui qt5-opengl qt5-printsupport qt5-svg qt5-websockets
h qt5-multimedia || true
h qt5-webengine || true
sudo pkg searh qt
fi
