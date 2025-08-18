#!/bin/sh
set -e

f() { sudo apt-get install --no-install-recommends -y "$@"; }
g() { sudo pkg_add "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux5" ] ; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libssh-gcrypt-4
f libpulse-dev
f qtbase5-dev qtbase5-dev-tools
f qtmultimedia5-dev libqt5multimediawidgets5
f libqt5opengl5 libqt5opengl5-dev
f libqt5svg5 libqt5svg5-dev
f qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "linux6" ] ; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libpulse-dev
f qt6-base-dev qt6-base-dev-tools
f qt6-multimedia-dev libqt6multimedia6 libqt6multimediawidgets6
f libqt6opengl6 libqt6opengl6-dev
f qt6-svg-dev libqt6svg6 libqt6svgwidgets6
f libqt6svg6 libqt6svgwidgets6 qt6-svg-dev
f libqt6webenginewidgets6 qt6-webengine-dev qt6-webengine-dev-tools
elif [ "$1" = "openbsd" ] ; then
g qtbase qtmultimedia qtsvg qttools qtwebengine qtwebsockets
elif [ "$1" = "freebsd" ] ; then
h qt5-qmake qt5-buildtools qt5-core
h qt5-gui qt5-opengl qt5-printsupport qt5-svg qt5-websockets qt5-multimedia qt5-webengine
fi
