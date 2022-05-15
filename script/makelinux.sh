#!/bin/sh
#
# arg is Qt version, e.g. "5.15.2"

export PATH=$GITHUB_WORKSPACE/qt/Qt/$1/gcc_64/bin:$PATH

run() {
 ./clean.l64
 cd lib
 qmake && make
 cd ../main
 qmake && make
 cd ..
 mv bin/linux-x86_64/release $1
 tar -czvf "$1".tar.gz $1
}

run jqt-linux

export JQTSLIM=1
run jqt-linux-slim
