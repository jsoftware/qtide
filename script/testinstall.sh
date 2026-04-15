#!/bin/sh
# set -evx

script/jinstall.sh -p qt1 -q slim --no-addons
find qt1 
script/jinstall.sh -p qt2 -q full --no-addons
find qt2 
script/qt1/j9.7/jqt.sh -js "exit 0[echo JVERSION"

