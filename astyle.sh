#!/bin/sh
set -e

export LC_ALL=en_US.UTF-8

if [ ! -f /usr/bin/astyle ] && [ ! -f /usr/local/bin/astyle ] && [ ! -f /opt/homebrew/bin/astyle ] ; then exit 0; fi

cd `dirname "$(realpath $0)"`
cd "$1"

f() {
 astyle \
  --style=linux \
  --indent=spaces=2 \
  --suffix=none \
  --quiet \
  --recursive \
  "$1"
}

f "*.cpp"
f "*.h"
