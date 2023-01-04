#!/bin/sh
set -e

#
# arg is Qt version, e.g. "5.15.2"
#        linux/raspberry/raspberry32/openbsd/freebsd

export PATH=$GITHUB_WORKSPACE/qt/Qt/$1/gcc_64/bin:$PATH

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ "$2" = "linux" ] || [ "$2" = "raspberry" ] || [ "$2" = "raspberry32" ]; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"

# following required on github actions, not needed on desktop:
cp -r lib/images .

if [ "$2" = "freebsd" ]; then
 export QMAKESPEC=freebsd-g++
fi

run() {
 ./clean.l64
 cd lib
 qmake && make
 cd ../main
 qmake && make
 cd ..
if [ "$2" = "linux" ]; then
 mv bin/linux-x86_64/release $1
 tar -czvf "$1".tar.gz $1
elif [ "$2" = "raspberry" ] || [ "$2" = "raspberry32" ]; then
 mv bin/linux-"`uname -m`"/release $1
 mkdir -p output
 tar -czvf output/"$1".tar.gz $1
elif [ "$2" = "openbsd" ]; then
 ls -l bin || true
 ls -l bin/openbsd-amd64 || true
 ls -l bin/openbsd-amd64/release || true
 mv bin/openbsd-amd64/release $1
 tar -czvf "$1".tar.gz $1
elif [ "$2" = "freebsd" ]; then
 ls -l bin || true
 ls -l bin/freebsd-amd64 || true
 ls -l bin/freebsd-amd64/release || true
 mv bin/freebsd-amd64/release $1
 tar -czvf "$1".tar.gz $1
fi
}

run jqt-"$2" "$2"

export JQTSLIM=1
run jqt-"$2"-slim "$2"
