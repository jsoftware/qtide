#!/bin/sh
set -e

S=$(dirname "$0")
cd $S

$S/astyle.sh

