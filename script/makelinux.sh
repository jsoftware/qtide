#!/bin/sh
set -e

#
# arg is Qt version, e.g. "5.15.2"
#        linux/raspberry/raspberry32/openbsd/freebsd

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
fi

run() {
QM="${QM:=qmake}"
hash $QM &> /dev/null
if [ $? -eq 1 ]; then
  echo 'use qmake-qt5' >&2
  QM=qmake-qt5
fi
./clean.l64
cd lib
$QM && make
cd ../main
$QM && make
cd ..
if [ "$2" = "linux" ]; then
 mv bin/linux-x86_64/release $1
 tar -czvf "$1".tar.gz $1
elif [ "$2" = "raspberry" ] || [ "$2" = "raspberry32" ]; then
 mv bin/linux-"`uname -m`"/release $1
 mkdir -p output
 tar -czvf output/"$1".tar.gz $1
elif [ "$2" = "openbsd" ]; then
 mv bin/openbsd-x86_64/release $1
 tar -czvf "$1".tar.gz $1
elif [ "$2" = "freebsd" ]; then
 mv bin/freebsd-x86_64/release $1
 tar -czvf "$1".tar.gz $1
fi
}

run jqt-"$2" "$2"

export JQTSLIM=1
run jqt-"$2"-slim "$2"
