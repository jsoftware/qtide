#!/bin/sh
set -e
cd "$(dirname "$0")"

# VER=6.5.3
VER=6.9.2

export ANDROID_SDK_ROOT=/Users/bill/Library/Android/sdk
export ANDROID_NDK_HOST=
export NDK_TOOLCHAIN_VERSION=
export ANDROID_NDK_PLATFORM=
export ANDROID_TARGET_ARCH=
export ANDROID_NDK_PLATFORM=android-28
export ANDROID_TARGET_ARCH=
export ANDROID_VERSION_CODE=
export ANDROID_API_VERSION=35 
export ANDROID_MIN_SDK_VERSION=10 
export ANDROID_TARGET_SDK_VERSION=35

export QMAKESPEC=android-clang
export JQTSLIM=JQTSLIM

ABI=arm64-v8a QM=$HOME/Qt/${VER}/android_arm64_v8a/bin/qmake ./makeall.sh "$@"
# ABI=armeabi-v7a QM=$HOME/Qt/${VER}/android_armv7/bin/qmake ./makeall.sh "$@"
# ABI=x86_64 QM=$HOME/Qt/${VER}/x86_64/bin/qmake ./makeall.sh "$@"
# ABI=x86 QM=$HOME/Qt/${VER}/x86/bin/qmake ./makeall.sh "$@"
