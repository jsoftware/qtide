#!/bin/bash

S=$(dirname "$0")
A=android-assets
P=~/jalgit/addons

cd $S

rm -rf $S/$A
mkdir -p $S/$A
cp -r $HOME/jalgit/base8/release/install/system $S/$A/.

mkdir -p $S/$A/bin
cp $HOME/jalgit/base8/main/config/profile.ijs $S/$A/bin/.
cp $HOME/jalgit/base8/main/config/profilex_template.ijs $S/$A/bin/.
echo "j802 install" > $S/$A/bin/installer.txt

function copyaddon() {
rm -rf $S/$A/addons/$1/$2
mkdir -p $S/$A/addons/$1
cp -r $P/$1/$2 $S/$A/addons/$1
}

copyaddon demos wd
copyaddon general misc
copyaddon graphics bmp
copyaddon graphics color
copyaddon graphics gl2
copyaddon gui wdclass
copyaddon ide qt

find $S/$A/addons \( -name '*.jproj' -o -name '*.dll' -o -name '*.exe' -o -name '*.so' -o -name '*.dylib' -o -name 'd3.v3.min.js' -o -name 'baselibtags' -o -name '.*' \) -delete
find $S/$A/addons/demos/wd -type f ! -name 'coins.ijs' -delete

rm -f $S/jqtdata.tgz
cd $S/$A
tar czf $S/jqtdata.tgz *
cd $S
mv jqtdata.tgz android_coins/assets/.
