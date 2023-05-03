#!/bin/sh
set -e

S=$(dirname "$0")
cd lib && ../astyle.sh && cd -
cd main && ../astyle.sh && cd -

