#!/bin/sh
set -evx

#
# arg is linux/raspberry/raspberry-arm32/openbsd/freebsd
# arg is Qt version, e.g. "5.15.2"

echo "pwd $(pwd)"
echo "parameters $1 $2"
echo "uname -m  $(uname -m)"
echo "RUNNER_ARCH  $RUNNER_ARCH"

B=$GITHUB_WORKSPACE/Qt/$2/gcc_64/bin
export PATH=$B:$PATH

case $2 in
6*) Qtver1="6" ;;
5*) Qtver1="5" ;;
*) Qtver1="4" ;;
esac

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ `uname`  = "Linux" ]; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"

# following required on github actions, not needed on desktop:
cp -r lib/images .

if [ "$1" = "openbsd" ]; then
# QM=/usr/local/lib/qt5/bin/qmake
 QM=/usr/local/lib/qt6/bin/qmake
elif [ "$1" = "freebsd" ]; then
# QM=/usr/local/lib/qt5/bin/qmake
 QM=/usr/local/lib/qt6/bin/qmake
elif [ "$1" = "linux" ]; then
 QM=$B/qmake
 export QMAKESPEC=linux-g++-64
 export JQTRPATH=JQTRPATH
# linux Qt 6.8.3 needs icu v73:
 cd icu
 tar -xvf icu4c-73_2-Ubuntu22.04-x64.tgz
 sudo cp -r icu/usr/local/lib/* /usr/lib/x86_64-linux-gnu
 cd -
fi

maketar() {
 cd $1
 tar -czvf ../"$1".tar.gz *
 cd -
}

run() {
./clean.l64
rm -rf "$1"
cd lib
$QM && make
cd -
cd main
$QM && make
cd -
if [ "$2" = "linux" ] || [ "$2" = "raspberry" ] || [ "$2" = "raspberry-arm32" ] ; then
 mv bin/linux-"`uname -m`"/release $1
 maketar $1
 if [ "`uname -m`" = "aarch64" -a "$RUNNER_ARCH" != "ARM64" ] || [ "`uname -m`" = "x86_64" -a "$RUNNER_ARCH" != "X64" ] || [ "`uname -m`" != "armv6l" -a "$2" = "raspberry-arm32" ] ; then
  mkdir -p output
  mv "$1".tar.gz output/.
 fi
elif [ "$2" = "openbsd" ]; then
 if [ "`uname -m`" = "amd64" ]; then
 mv bin/openbsd-x86_64/release $1
 else
 mv bin/openbsd-aarch64/release $1
 fi
 maketar $1
elif [ "$2" = "freebsd" ]; then
 if [ "`uname -m`" = "amd64" ]; then
 mv bin/freebsd-x86_64/release $1
 else
 mv bin/freebsd-aarch64/release $1
 fi
 maketar $1
fi
ls -l "$1" || true
rm -rf "$1"
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
run jqt-"$1" "$1"

export JQTSLIM=1
run jqt-"$1"-slim "$1"

# if [ -d Qt ] ; then
# tar -czf "$1"-Qt.tar.gz Qt
# fi

if [ "$1" = "linux" ]; then
  cat common.pri | grep "^VERSION" > version.txt
  cat common.pri | grep "^JDLLVER" >> version.txt
fi

./clean.l64 || true
