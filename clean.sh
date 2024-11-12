#!/bin/sh
set -e

exit 0
S=$(dirname "$0")
cd lib && ../astyle.sh && cd -
cd main && ../astyle.sh && cd -

