#!/bin/sh
set -evx

# argument is linux|darwin|raspberry|android|openbsd|freebsd|wasm [x64|x86|arm64|armv6l]
# wasm is experimental

echo "pwd $(pwd)"
echo "parameters $1 $2"
echo "uname -m  $(uname -m)"
echo "RUNNER_ARCH  $RUNNER_ARCH"

if [ "$2" = "x64" ]; then
 arch=x86_64
elif [ "$2" = "x86" ]; then
 arch=x86
elif [ "$2" = "arm64" ]; then
 arch=aarch64
elif [ "$2" = "armv6l" ]; then
 arch=armv6l
elif [ "$1" = "wasm" ]; then
 arch=wasm32
else
 arch="$(uname -m)"
fi

f() {
 sudo apt-get install --no-install-recommends -y "$@" || true
 if [ $arch = "armv6l" ]; then
  if [ "$RUNNER_ARCH" = "ARM64" ]; then
   sudo apt-get install --no-install-recommends -y "$@":armhf || true
  fi
 fi
}
g() { sudo pkg_add "$@"; }
h() { sudo pkg install -y "$@"; }

if [ "$1" = "linux" ] || [ "$1" = "raspberry" ]; then
 f libasound2
 f libegl1
 f libegl1-mesa
 f libgl1-mesa-dri
 f libopus0
 f libpulse0
 f libxcb-cursor0
 f libxcb-xinerama0
 f minizip
 f pipewire
 f zlib1g
elif [ "$1" = "openbsd" ]; then
 g libxcb-cursor0
elif [ "$1" = "freebsd" ]; then
 g libxcb-cursor0
fi
