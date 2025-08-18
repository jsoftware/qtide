#!/bin/sh
set -e

f() { sudo apt-get install --no-install-recommends -y "$@"; }
g() { sudo pkg_add "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux" ] ; then
sudo apt-get update -y
f libssh-gcrypt-4
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "raspberry" ] ; then
sudo apt-get update -y
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "raspberry-arm32" ] ; then
sudo apt-get update -y
# f libqt4-dev libqt4-opengl-dev libqt4-svg
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "openbsd" ] ; then
g qtbase qtmultimedia qtsvg qttools qtwebengine qtwebsockets
elif [ "$1" = "freebsd" ] ; then
h qt5-qmake qt5-buildtools qt5-core
h qt5-gui qt5-opengl qt5-printsupport qt5-svg qt5-websockets qt5-multimedia qt5-webengine
fi
