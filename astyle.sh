#!/bin/bash

if [ ! -f /usr/bin/astyle ]; then exit 0; fi

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
