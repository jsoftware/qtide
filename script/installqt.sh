#!/bin/sh
set -evx

# argument is linux|darwin|raspberry|android|openbsd|freebsd|wasm [x64|x86|arm64|armv6l] Qtver
# wasm is experimental
# Qt version, e.g. "5.15.2"

echo "pwd $(pwd)"
echo "parameters $1 $2 $3"
echo "uname -m  $(uname -m)"
echo "RUNNER_ARCH  $RUNNER_ARCH"

case $3 in
6*) Qtver1="6" ;;
5*) Qtver1="5" ;;
*) Qtver1="4" ;;
esac

if [ "$2" = "x64" ] ; then
 arch=x86_64
elif [ "$2" = "x86" ] ; then
 arch=x86
elif [ "$2" = "arm64" ] ; then
 arch=aarch64
elif [ "$2" = "armv6l" ] ; then
 arch=armv6l
elif [ "$1" = "wasm" ]; then
 arch=wasm32
else
 arch="`uname -m`"
fi

f() {
sudo apt-get install --no-install-recommends -y "$@" || true ;
if [ $arch = "armv6l" ] ; then
if [ "$RUNNER_ARCH" = "ARM64" ] ; then
sudo apt-get install --no-install-recommends -y "$@":armhf || true ;
end.
fi
}
g() { sudo pkg_add "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux" ] || [ "$1" = "raspberry" ] ; then
if [ $Qtver1 = "4" ] ; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libqt4-dev
f libqt4-opengl-dev
f libqt4-svg
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
f libqt5webkit5-dev
f libqt5websockets5-dev
elif [ $Qtver1 = "6" ] ; then
sudo apt-get update -y
sudo apt-get upgrade -y
f libpulse-dev
f qmake6
f libqt6core5compat6
f libqt6multimedia6
f libqt6multimediawidgets6
f libqt6opengl6
f libqt6opengl6-dev
f libqt6svg6
f libqt6svgwidgets6
f libqt6webenginewidgets6
f libqt6websockets6
f qt6-5compat-dev
f qt6-base-dev
f qt6-multimedia-dev
f qt6-svg-dev
f qt6-webengine-dev
f qt6-websockets-dev
fi
elif [ "$1" = "openbsd" ] ; then
if [ $Qtver1 = "5" ] ; then
g qtbase qtmultimedia qtsvg qttools qtwebengine qtwebsockets
else
g qt6 qt6-qt5compat
g qt6-qtsvg qt6-qtwebsockets qt6-qtmultimedia qt6-qtwebengine
fi
elif [ "$1" = "freebsd" ] ; then
if [ $Qtver1 = "5" ] ; then
h qt5-qmake qt5-buildtools qt5-core
h qt5-gui qt5-opengl qt5-printsupport qt5-svg qt5-websockets qt5-multimedia qt5-webengine
else
h qt6 qt6-base qt6-5compat
h qt6-svg qt6-websockets qt6-multimedia qt6-webengine
fi
fi

