#!/bin/bash

S=~/dev/apps/ide/jqt

echo "updating repos"
cd ~/jalgit/base8
git svn rebase
cd ~/jalgit/addons
git svn rebase

A=android-assets
P=~/jalgit/addons

cd $S

echo "building base library"
./buildbase.sh

rm -rf $S/$A
mkdir -p $S/$A
cp -r $HOME/jalgit/base8/release/install/system $S/$A/.

mkdir -p $S/$A/bin
cp $HOME/jalgit/base8/main/config/profile.ijs $S/$A/bin/.
cp $HOME/jalgit/base8/main/config/profilex_template.ijs $S/$A/bin/.
echo "j804 install" > $S/$A/bin/installer.txt

function copyaddon() {
rm -rf $S/$A/addons/$1/$2
mkdir -p $S/$A/addons/$1
cp -r $P/$1/$2 $S/$A/addons/$1
}

# copyaddon api android
copyaddon api expat
# copyaddon api gles
copyaddon api jni
# copyaddon api sl4a
# copyaddon arc zlib
copyaddon convert json
# copyaddon data jmf
# copyaddon demos isigraph
# copyaddon demos wd
# copyaddon demos wdplot
# copyaddon docs help
# copyaddon games minesweeper
# copyaddon games nurikabe
# copyaddon games pousse
# copyaddon games solitaire
# copyaddon general misc
# copyaddon graphics afm
# copyaddon graphics bmp
# copyaddon graphics color
copyaddon graphics gl2
# copyaddon graphics graph
# copyaddon graphics grid
# copyaddon graphics plot
# copyaddon graphics png
# copyaddon graphics print
# copyaddon graphics viewmat
copyaddon gui android
copyaddon ide jhs
copyaddon ide qt
# copyaddon labs labs
# copyaddon math deoptim
# copyaddon math misc
# copyaddon stats base

find $A/addons \( -name '*.jproj' -o -name '*.dll' -o -name '*.exe' -o -name '*.so' -o -name '*.dylib' -o -name 'd3.v3.min.js' -o -name 'baselibtags' -o -name '.*' \) -delete

rm -f jqtdata.tgz
cd $A
tar czf ../jqtdata.tgz *
