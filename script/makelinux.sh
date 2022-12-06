#!/bin/sh
#
# arg is Qt version, e.g. "5.15.2"

export PATH=$GITHUB_WORKSPACE/qt/Qt/$1/gcc_64/bin:$PATH

if [ "x$MAKEFLAGS" = x'' ] ; then
if [ "$2" != "darwin" ]; then par=`nproc`; else par=`sysctl -n hw.ncpu`; fi
export MAKEFLAGS=-j$par
fi
echo "MAKEFLAGS=$MAKEFLAGS"

# following required on github actions, not needed on desktop:
cp -r lib/images .

run() {
 ./clean.l64
 cd lib
 qmake && make
 cd ../main
 qmake && make
 cd ..
if [ "$2" = "linux" ]; then
 mv bin/linux-x86_64/release $1
else
 mv bin/linux-"`uname -m`"/release $1
fi
 tar -czvf "$1".tar.gz $1
}

run jqt-"$2"

export JQTSLIM=1
run jqt-"$2"-slim
