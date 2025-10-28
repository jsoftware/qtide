#!/bin/sh
set -evx

#
# arg is linux/raspberry/raspberry-arm32/openbsd/freebsd
#        Qt version, e.g. "5.15.2"

echo "pwd $(pwd)"
echo "parameters $1 $2"
echo "uname -m  $(uname -m)"
echo "RUNNER_ARCH  $RUNNER_ARCH"

case $2 in
6*) Qtver1="6" ;;
5*) Qtver1="5" ;;
*) Qtver1="4" ;;
esac

f() {
sudo apt-get install --no-install-recommends -y "$@";
sudo apt-get install --no-install-recommends -y "$@":armhf || true ;
}
g() { sudo pkg_add "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux" ] || [ "$1" = "raspberry" ] || [ "$1" = "raspberry-arm32" ] ; then
if [ $Qtver1 = "4" ] ; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libqt4-dev libqt4-opengl-dev libqt4-svg
elif [ $Qtver1 = "5" ] ; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libssh-gcrypt-4
f libpulse-dev
f qtbase5-dev
f qtmultimedia5-dev
f libqt5multimediawidgets5
f libqt5opengl5
f libqt5opengl5-dev
f libqt5svg5
f libqt5svg5-dev
f qtwebengine5-dev
f libqt5websockets5-dev
elif [ $Qtver1 = "6" ] ; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libpulse-dev
f qmake6
f qt6-base-dev
f qt6-multimedia-dev
f libqt6multimedia6
f libqt6multimediawidgets6
f libqt6opengl6
f libqt6opengl6-dev
f libqt6svg6
f libqt6svgwidgets6
f libqt6webenginewidgets6
f qt6-webengine-dev
f libqt6core5compat6
f libqt6websockets6
fi
elif [ "$1" = "openbsd" ] ; then
g qtbase qtmultimedia qtsvg qttools qtwebengine qtwebsockets
elif [ "$1" = "freebsd" ] ; then
# h qt5-qmake qt5-buildtools qt5-core
# h qt5-gui qt5-opengl qt5-printsupport qt5-svg qt5-websockets qt5-multimedia qt5-webengine
h qt6 qt6-base qt6-5compat
h qt6-svg qt6-websockets qt6-multimedia qt6-webengine
fi

