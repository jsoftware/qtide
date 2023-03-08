#!/bin/sh
set -e

#
# arg is Qt version, e.g. "5.15.2"
#        linux/raspberry/raspberry-arm32/openbsd/freebsd

export PATH=$GITHUB_WORKSPACE/qt/Qt/$1/gcc_64/bin:$PATH

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ `uname`  = "Linux" ]; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"

# following required on github actions, not needed on desktop:
cp -r lib/images .

if [ "$2" = "openbsd" ]; then
 QM=/usr/local/lib/qt5/bin/qmake
elif [ "$2" = "freebsd" ]; then
 QM=/usr/local/lib/qt5/bin/qmake
 export JQTWEBKIT=JQTWEBKIT
fi

maketar() {
 cd $1
 tar -czvf ../"$1".tar.gz *
 cd -
}

run() {
QM="${QM:=qmake}"
hash $QM &> /dev/null
if [ $? -eq 1 ]; then
  echo 'use qmake-qt5' >&2
  QM=qmake-qt5
fi
./clean.l64
rm -rf "$1"
cd lib
$QM && make
cd ../main
$QM && make
cd ../amalgam
$QM && make
cd ..
if [ "$2" = "linux" ]; then
 mv bin/linux-x86_64/release $1
 maketar $1
elif [ "$2" = "raspberry" ] || [ "$2" = "raspberry-arm32" ]; then
 mv bin/linux-"`uname -m`"/release $1
 maketar $1
 mkdir -p output
 mv "$1".tar.gz output/.
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
ls -l "$1"
rm -rf "$1"
}

run jqt-"$2" "$2"

export JQTSLIM=1
run jqt-"$2"-slim "$2"

./clean.l64 || true
