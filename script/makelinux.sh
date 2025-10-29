#!/bin/sh
set -evx

# argument is linux|darwin|raspberry|android|openbsd|freebsd|wasm [x64|x86|arm64|armv6l] Qtver
# wasm is experimental
# Qt version, e.g. "5.15.2"

echo "pwd $(pwd)"
echo "parameters $1 $2 $3"
echo "uname -m  $(uname -m)"
echo "RUNNER_ARCH  $RUNNER_ARCH"

export QTLIBVER="$3"

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

echo "`ls -l $GITHUB_WORKSPACE/Qt/$3`"
B=$GITHUB_WORKSPACE/Qt/$3/gcc_64/bin
export PATH=$B:$PATH

case $3 in
6*) Qtver1="6" ;;
5*) Qtver1="5" ;;
*) Qtver1="4" ;;
esac

echo "MAKEFLAGS=$MAKEFLAGS"
if [ "x$MAKEFLAGS" = x'' ] ; then
 if [ "$1" = "wasm" ] ; then
  par=2
 elif [ "$1" = "linux" ] || [ "$1" = "raspberry" ] ; then
  par=`nproc`
 elif [ "$1" = "darwin" ] || [ "$1" = "openbsd" ] || [ "$1" = "freebsd" ] || [ "$1" = "android" ] ; then
  par=`sysctl -n hw.ncpu`
 else
  par=2
 fi
 export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"

# following required on github actions, not needed on desktop:
cp -r lib/images .

if [ "$1" = "openbsd" ]; then
if [ $Qtver1 = "6" ]; then
 QM=/usr/local/lib/qt6/bin/qmake
elif [ $Qtver1 = "5" ]; then
 QM=/usr/local/lib/qt5/bin/qmake
else
 QM=/usr/local/lib/qt4/bin/qmake
fi
elif [ "$1" = "freebsd" ]; then
if [ $Qtver1 = "6" ]; then
 QM=/usr/local/lib/qt6/bin/qmake
elif [ $Qtver1 = "5" ]; then
 QM=/usr/local/lib/qt5/bin/qmake
else
 QM=/usr/local/lib/qt4/bin/qmake
fi
elif [ "$1" = "raspberry" ]; then
if [ "$2" = "armv6l" ] ; then
 export QMAKESPEC=linux-arm-gnueabi-g++
 SUF=-arm32
fi
elif [ "$1" = "linux" ]; then
 QM=$B/qmake
 export QMAKESPEC=linux-g++-64
 export JQTRPATH=JQTRPATH
# linux Qt 6.8.3 needs icu v73:
if [ $Qtver1 = "6" ]; then
 cd icu
 tar -xvf icu4c-73_2-Ubuntu22.04-x64.tgz
 sudo cp -r icu/usr/local/lib/* /usr/lib/x86_64-linux-gnu
 cd -
fi
fi

maketar() {
 cd $1
 tar -czvf ../"$1".tar.gz *
 cd -
}

run() {
P=jqt-"$1""$SUF"
if [ "$JQTSLIM" = "1" ] ; then
P="$P"-slim
fi

./clean.l64
rm -rf "$P"
cd lib
$QM && make
cd -
cd main
$QM && make
cd -
if [ "$1" = "linux" ] || [ "$1" = "raspberry" ] ; then
 mv bin/linux-$arch/release "$P"
 maketar "$P"
 if [ "`uname -m`" = "aarch64" -a "$RUNNER_ARCH" != "ARM64" ] || [ "`uname -m`" = "x86_64" -a "$RUNNER_ARCH" != "X64" ] || [ "`uname -m`" != "armv6l" -a "$1" = "raspberry-arm32" ] ; then
  mkdir -p output
  mv "$P".tar.gz output/.
 fi
elif [ "$1" = "openbsd" ]; then
 mv bin/openbsd-$arch/release "$P"
 maketar "$P"
elif [ "$1" = "freebsd" ]; then
 mv bin/freebsd-$arch/release "$P"
 maketar "$P"
fi
ls -l "$P" || true
rm -rf "$P"
}

if [ $Qtver1 = "6" ]; then
QM="${QM:=qmake6}"
elif [ $Qtver1  = "5" ]; then
QM="${QM:=qmake}"
hash $QM &> /dev/null
if [ $? -eq 1 ]; then
  echo 'use qmake-qt5' >&2
  QM=qmake-qt5
fi
else
QM="${QM:=qmake}"
fi

# export NO_OPENGL=1
run "$1"

export JQTSLIM=1
run "$1"

# if [ -d Qt ] ; then
# tar -czf "$1"-Qt.tar.gz Qt
# fi

if [ "$1" = "linux" ]; then
  cat common.pri | grep "^VERSION" > version.txt
  cat common.pri | grep "^JDLLVER" >> version.txt
fi

./clean.l64 || true
