#!/bin/sh
set -e

#
# arg is linux/openbsd/freebsd
#        Qt major version, e.g. "5"

Qtver1= ${"$1":0:1}

f() { sudo apt-get install --no-install-recommends -y "$@"; }
g() { sudo pkg_add "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux" ] ; then
if [ $Qtver1 == "5" ]; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libssh-gcrypt-4
f libpulse-dev
f qtbase5-dev
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ $Qtver1 == "6" ]; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libpulse-dev
f qmake6
f qt6-base-dev
f qt6-multimedia-dev libqt6multimedia6 libqt6multimediawidgets6
f libqt6opengl6 libqt6opengl6-dev
f libqt6svg6 libqt6svgwidgets6
f libqt6webenginewidgets6 qt6-webengine-dev
fi
elif [ "$1" = "openbsd" ] ; then
g qtbase qtmultimedia qtsvg qttools qtwebengine qtwebsockets
elif [ "$1" = "freebsd" ] ; then
h qt5-qmake qt5-buildtools qt5-core
h qt5-gui qt5-opengl qt5-printsupport qt5-svg qt5-websockets qt5-multimedia qt5-webengine
fi
