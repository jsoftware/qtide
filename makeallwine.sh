#!/bin/sh

export QT_SELECT=4

./makeall.sh
./makeall32.sh
./makewine64.sh
./makewine32.sh
./makeraspi.sh
