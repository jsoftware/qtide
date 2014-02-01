#!/bin/bash

S=~/dev/apps/ide/jqt

cd $S

rm -rf $S/android-assets
mkdir -p $S/android-assets
cp -r $HOME/jalgit/base8/release/install/system $S/android-assets/.

P=~/jalgit/addons/graphics
Q=$S/android-assets/addons/graphics
mkdir -p $Q
cp -r $P/afm $Q
cp -r $P/bmp $Q
cp -r $P/color $Q 
cp -r $P/gl2 $Q
cp -r $P/grid $Q
cp -r $P/plot $Q
cp -r $P/print $Q
cp -r $P/viewmat $Q

P=~/jalgit/addons/data
Q=$S/android-assets/addons/data
mkdir -p $Q
cp -r $P/jmf $Q

P=~/jalgit/addons/ide
Q=$S/android-assets/addons/ide
mkdir -p $Q
cp -r $P/jhs $Q
cp -r $P/qt $Q

P=~/jalgit/addons/demos
Q=$S/android-assets/addons/demos
mkdir -p $Q
cp -r $P/isigraph $Q
cp -r $P/wd $Q
cp -r $P/wdplot $Q

P=~/jalgit/addons/games
Q=$S/android-assets/addons/games
mkdir -p $Q
cp -r $P/minesweeper $Q
cp -r $P/nurikabe $Q
cp -r $P/pousse $Q
cp -r $P/solitaire $Q

P=~/jalgit/addons/api
Q=$S/android-assets/addons/api
mkdir -p $Q
cp -r $P/android $Q
cp -r $P/expat $Q
cp -r $P/gles $Q
cp -r $P/jni $Q
cp -r $P/sl4a $Q

P=~/jalgit/addons/gui
Q=$S/android-assets/addons/gui
mkdir -p $Q
cp -r $P/wdclass $Q

P=~/jalgit/addons/convert
Q=$S/android-assets/addons/convert
mkdir -p $Q
cp -r $P/json $Q

P=~/jalgit/addons/math
Q=$S/android-assets/addons/math
mkdir -p $Q
cp -r $P/deoptim $Q
cp -r $P/misc $Q

P=~/jalgit/addons/general
Q=$S/android-assets/addons/general
mkdir -p $Q
cp -r $P/misc $Q
cp -r $P/scriptdoc $Q

P=~/jalgit/addons/stats
Q=$S/android-assets/addons/stats
mkdir -p $Q
cp -r $P/base $Q

find $S/android-assets/addons \( -name '*.jproj' -o -name '*.dll' -o -name '*.exe' -o -name '*.so' -o -name '*.dylib' \) -delete

cd $S/android-assets
tar czf ../jqtdata.tgz *
cd $S
cp jqtdata.tgz android_qtide/assets/.
cp $HOME/jalgit/base8/main/config/profile.ijs android_qtide/assets/.
cp $HOME/jalgit/base8/main/config/profilex_template.ijs android_qtide/assets/profilex.ijs
sed -i -e '/^NB\./!d' android_qtide/assets/profilex.ijs
