#!/bin/bash

export LC_ALL=en_US.UTF-8

if [ ! -f /usr/bin/astyle ]; then exit 0; fi

f() {
 astyle \
  --style=linux \
  --indent=spaces=2 \
  --suffix=none \
  --quiet \
  --exclude=lib/build \
  --exclude=main/build \
  --recursive \
  "$1"
}

f "*.cpp"
f "*.h"
