#!/bin/sh

f() { sudo apt-get install --no-install-recommends -y "$@"; }
g() { sudo pkg_add install "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux" ] ; then
f qt5-default
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
g libpulse-dev
g qtbase5-dev qtbase5-dev-tools
g qtmultimedia5-dev libqt5multimediawidgets5
g libqt5opengl5 libqt5opengl5-dev
g libqt5svg5 libqt5svg5-dev
g qtwebengine5-dev libqt5websockets5-dev
elif [ "$1" = "freebsd" ] ; then
# h qmake qt6-3d qt6-5compat qt6-base qt6-charts qt6-datavis3d qt6-declarative qt6-imageformats qt6-multimedia qt6-networkauth qt6-positioning qt6-quick3d qt6-quicktimeline qt6-remoteobjects qt6-shadertools qt6-svg qt6-wayland qt6-webchannel qt6-websockets
h qmake qt6
fi
