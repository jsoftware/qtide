#!/bin/sh
# set -evx

# argument is linux|darwin|raspberry|android|openbsd|freebsd|wasm [x64|x86|arm64|armv6l]
# wasm is experimental

echo "pwd $(pwd)"
echo "parameters $1 $2"
echo "uname -m  $(uname -m)"
echo "RUNNER_ARCH  $RUNNER_ARCH"

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

script/jinstall.sh -p qt1 -q slim --no-addons
find qt1 
script/jinstall.sh -p qt2 -q full --no-addons
find qt2 
if [ "$1" = "darwin" ] ; then
  if [ "$arch" = "x86_64" ] ; then
    ./qt1/j9.7/bin/jqtx.command -js "exit 0[stderr JVERSION"
    ./qt2/j9.7/bin/jqtx.command -js "exit 0[stderr JVERSION"
  else
    ./qt1/j9.7/bin/jqt.command -js "exit 0[stderr JVERSION"
    ./qt2/j9.7/bin/jqt.command -js "exit 0[stderr JVERSION"
  fi
else
  ./qt1/j9.7/jqt.sh -js "exit 0[stderr JVERSION"
  ./qt2/j9.7/jqt.sh -js "exit 0[stderr JVERSION"
fi

