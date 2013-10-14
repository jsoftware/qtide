#!/bin/sh

export QT_SELECT=4

./makeall.sh
./makeall32.sh
./makewin64.sh
./makewin.sh
./makeraspi.sh
