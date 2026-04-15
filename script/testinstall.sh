#!/bin/sh
# set -evx

# argument is linux|darwin|raspberry|android|openbsd|freebsd|wasm [x64|x86|arm64|armv6l] Qtver
# wasm is experimental
# Qt version, e.g. "5.15.2"

echo "pwd $(pwd)"
echo "parameters $1 $2 $3"
echo "uname -m  $(uname -m)"
echo "RUNNER_ARCH  $RUNNER_ARCH"

script/jinstall.sh -p qt1 -q slim --no-addons
find qt1 
script/jinstall.sh -p qt2 -q full --no-addons
find qt2 
if [ "$1" = "darwin" ] ; then
  ./qt1/j9.7/jqt.app -js "exit 0[echo JVERSION"
else
  ./qt1/j9.7/jqt.sh -js "exit 0[echo JVERSION"
fi

